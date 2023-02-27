#include "../set.h"
#include <thread>
#include <cstdint>
using namespace std;

template <typename T> class FineSet : public Set<T> {
  public:
  FineSet() {
    this->head = new Node<T>();
    this->head->key = 0;
    this->head->next = new Node<T>();
    this->head->next->key = SIZE_MAX;
  }

  ~FineSet() {
    Node<T> *current = this->head;
    while (current != NULL) {
      Node<T> *next = current->next;
      delete current;
      current = next;
    }
  }

  bool add(T element) override {
    bool success;
    size_t key = hash<T>()(element);
    this->head->mutex.lock();
    Node<T> *pred = this->head;
    try {
      Node<T> *curr = pred->next;
      curr->mutex.lock();
      try {
        while (curr->key < key) {
          pred->mutex.unlock();
          pred = curr;
          curr = curr->next;
          curr->mutex.lock();
        }
        if (curr->key == key) {
          throw 505;
        }
        Node<T> *newNode = new Node<T>(element);
        newNode->next = curr;
        pred->next = newNode;
        success = true;
        printf("add(%d) completed by thread - %d \n", element,
               this_thread::get_id());
      } catch (...) {
        success = false;
        printf("add(%d) unsuccessful by thread - %d \n", element,
               this_thread::get_id());
      }
      curr->mutex.unlock();
    } catch (...) {
    }
    pred->mutex.unlock();

    return success;
  };

  bool remove(T element) override {
    bool success = false;
    Node<T> *pred = nullptr;
    Node<T> *curr = nullptr;
    size_t key = hash<T>()(element);
    this->head->mutex.lock();
    try {
      pred = this->head;
      curr = pred->next;
      curr->mutex.lock();
      try {
        while(curr->key < key){
          pred->mutex.unlock();
          pred = curr;
          curr = curr->next;
          curr->mutex.lock();
        }
        if (curr->key == key){
          pred->next = curr->next;
          success = true;
          printf("remove(%d) completed by thread - %d \n", element,
               this_thread::get_id());
        }else{
          throw 505;
        }
      } catch (...) {
        success = false;
        printf("remove(%d) unsuccessful by thread - %d \n", element,
               this_thread::get_id());
      }
      curr->mutex.unlock();
    } catch (...) {
    }
    pred->mutex.unlock();

    return success;
  };

  bool contains(T element) override{
    bool success = false;
    Node<T> *curr = nullptr;
    Node<T> *pred = nullptr;
    size_t key = hash<T>()(element);
    this->head->mutex.lock();
    try{
      pred = this->head;
      curr = pred->next;
      curr->mutex.lock();
      while(curr->key < key){
        pred->mutex.unlock();
        pred = curr;
        curr = curr->next;
        curr->mutex.lock();        
      }
      if (key == curr->key){
        success = true;
        printf("contains(%d) completed by thread - %d \n", element,
               this_thread::get_id());
      }else{
        throw 505;
      }
    }catch(...){
        printf("contains(%d) unsuccessful - %d \n", element,
               this_thread::get_id());
      success = false;
    }
    curr->mutex.unlock();
    pred->mutex.unlock();

    return success;
    
  };

  int size() override{
    Node<T> *current = this->head;
    int count = 0;
    while (current != NULL) {
      Node<T> *next = current->next;
      count++;
      current = next;
    }
    return count-2; //ignore the default min limit and max limit set in place 
  }
};