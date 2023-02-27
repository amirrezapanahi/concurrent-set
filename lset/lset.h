#include "../set.h"
#include <cstdint>
#include <thread>
using namespace std;

template <typename T> class LazySet : public Set<T> {
public:
  LazySet() {
    this->head = new Node<T>();
    this->head->key = 0;
    this->head->next = new Node<T>();
    this->head->next->key = SIZE_MAX;
  }

  ~LazySet() {
    Node<T> *current = this->head;
    while (current != NULL) {
      Node<T> *next = current->next;
      delete current;
      current = next;
    }
  }

  bool add(T element) override {
    bool success = false;
    size_t key = hash<T>()(element);
    while (true) {
      Node<T> *pred = this->head;
      Node<T> *curr = this->head->next;
      while (curr->key < key) {
        pred = curr;
        curr = curr->next;
      }

      pred->mutex.lock();
      curr->mutex.lock();

      if (validate(pred, curr)) {
        if (curr->key == key) {
          success = false;
          printf("add(%d) unsuccessful by thread - %d \n", element,
                 this_thread::get_id());
        } else {
          Node<T> *node = new Node<T>(element);
          node->next = curr;
          pred->next = node;
          success = true;
          printf("add(%d) completed by thread - %d \n", element,
                 this_thread::get_id());
        }
      }

      curr->mutex.unlock();
      pred->mutex.unlock();

      return success;
    }
  }

  bool remove(T element) override {
    bool success = false;
    size_t key = hash<T>()(element);
    while (true) {
      Node<T> *pred = this->head;
      Node<T> *curr = this->head->next;
      while (curr->key < key) {
        pred = curr;
        curr = curr->next;
      }

      pred->mutex.lock();
      curr->mutex.lock();

      if (validate(pred, curr)) {
        if (curr->key != key) {
          success = false;
          printf("remove(%d) unsuccessful by thread - %d \n", element,
                 this_thread::get_id());
        } else {
          curr->marked = true;
          pred->next = curr->next;
          success = true;
          printf("remove(%d) completed by thread - %d \n", element,
                 this_thread::get_id());
        }
      }

      curr->mutex.unlock();
      pred->mutex.unlock();

      return success;
    }
  };

  bool contains(T element) override {
    size_t key = hash<T>()(element);
    Node<T> *curr = this->head;
    while (curr->key < key) {
      curr = curr->next;
    }

    if (curr->key == key && !curr->marked) {
      printf("contains(%d) completed by thread - %d \n", element,
             this_thread::get_id());
      return true;
    }else{
      printf("contains(%d) unsuccessful by thread - %d \n", element,
             this_thread::get_id());
      return false;
    }
  };

  int size() override {
    Node<T> *current = this->head;
    int count = 0;
    while (current != NULL) {
      Node<T> *next = current->next;
      count++;
      current = next;
    }
    return count - 2; // ignore the default min limit and max limit set in place
  }

private:
  bool validate(Node<T> *pred, Node<T> *curr) {
    return ((!pred->marked) && (!curr->marked) && (pred->next == curr));
  }
};