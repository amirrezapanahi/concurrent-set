#include "../set.h"
#include <cstdint>
#include <thread>
using namespace std;

template <typename T> class Window {
public:
  Node<T> *pred, *curr;

  Window(Node<T> *myPred, Node<T> *myCurr) {
    pred = myPred;
    curr = myCurr;
  }
};

template <typename T> Window<T> find(Node<T> *head, size_t key) {
  Node<T> *pred = nullptr, *curr = nullptr, *succ = nullptr;
  bool marked = false;
  bool snip;

retry:

  while (true) {
    pred = head;

    if (pred->atmc_next) {
      curr = (Node<T> *)(pred->atmc_next->getRef());
    }

    while (true) {
      if (curr->atmc_next != nullptr) {
        succ = (Node<T> *)(curr->atmc_next->get(&marked));
      } else {
        marked = false;
      }

      while (marked) {
        if (pred->atmc_next == NULL) {
          printf("\nNo entry");
          while (true)
            ;
        }

        snip = pred->atmc_next->compareAndSet((UINT64)curr, (UINT64)succ, false,
                                              false);
        if (!snip) {
          goto retry;
        }
        curr = succ;

        if (curr->atmc_next) {
          succ = (Node<T> *)(curr->atmc_next->get(&marked));
        } else {
          marked = false;
        }
      }

      if ((curr->key) >= key) {
        Window<T> *pWindow = new Window<T>(pred, curr);
        return *pWindow;
      }

      pred = curr;
      curr = succ;
    }
  }
}

template <typename T> class NonBlockingSet : public Set<T> {
public:
  NonBlockingSet() {
    Node<T> *nextNode = new Node<T>();
    nextNode->key = SIZE_MAX;
    this->head = new Node<T>();
    this->head->key = 0;
    this->head->atmc_next =
        new AtomicMarkableReference((UINT64)nextNode, false);
  }

  ~NonBlockingSet() {
    // Node<T> *current = this->head;
    // while (current != nullptr) {
    //   AtomicMarkableReference *next = current->atmc_next;
    //   delete current;
    //   current = ;
    // }
    delete this->head->next;
    delete this->head;
  }

  bool add(T element) override {
    size_t key = hash<T>()(element);
    while (true) {
      Window<T> window = find(this->head, key);
      Node<T> *pred = window.pred;
      Node<T> *curr = window.curr;
      if (curr->key == key) {
        printf("add(%d) unsuccessful by thread - %d \n", element,
               this_thread::get_id());
        return false;
      } else {
        Node<T> *node = new Node<T>(element);
        node->atmc_next = new AtomicMarkableReference((UINT64)curr, false);
        if (pred->atmc_next->compareAndSet((UINT64)curr, (UINT64)node, false,
                                           false)) {
          printf("add(%d) completed by thread - %d \n", element,
                 this_thread::get_id());
          return true;
        }
      }
    }
  }

  bool remove(T element) override {
    size_t key = hash<T>()(element);
    bool snip;
    while (true) {
      Window<T> window = find(this->head, key);
      Node<T> *pred = window.pred;
      Node<T> *curr = window.curr;
      if (curr->key != key) {
        printf("remove(%d) unsuccessful by thread - %d \n", element,
               this_thread::get_id());
        return false;
      } else {
        Node<T> *succ = (Node<T> *)curr->atmc_next->getRef();
        snip = curr->atmc_next->attemptMark((UINT64)succ, true);
        if (!snip) {
          continue;
        }
        pred->atmc_next->compareAndSet((UINT64)curr, (UINT64)succ, false,
                                       false);
        printf("remove(%d) completed by thread - %d \n", element,
               this_thread::get_id());
        return true;
      }
    }
  }

  bool contains(T element) override {
    bool marked = false;
    size_t key = hash<T>()(element);
    Node<T> *curr = this->head;
    while (curr->key < key) {
      Node<T> *succ = nullptr;
      if (curr->atmc_next) {
        curr = (Node<T> *)curr->atmc_next->getRef();
        if (curr->atmc_next) {
          succ = (Node<T> *)curr->atmc_next->get(&marked);
        }
      }
    }
    if (curr->key == key && !marked) {
      printf("contains(%d) completed by thread - %d \n", element,
             this_thread::get_id());
      return true;
    } else {
      printf("contains(%d) unsuccessful by thread - %d \n", element,
             this_thread::get_id());
      return false;
    }
  }

  int size() override {
    Node<T> *current = this->head;
    int count = 0;
    while (current != nullptr) {
      if (current->atmc_next) {
        Node<T> *next = (Node<T> *)current->atmc_next->getRef();
        count++;
        current = next;
      } else {
        break;
      }
    }
    return count - 2; // ignore the default min limit and max limit set in place
  }
};