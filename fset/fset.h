#include "../set.h"
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
      } catch (...) {
        success = false;
      }
      curr->mutex.unlock();
    } catch (...) {
    }
    pred->mutex.unlock();

    if (success){
      return true;
    }else{
      return false;
    }
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
        }else{
          throw 505;
        }
      } catch (...) {
        success = false;
      }
      curr->mutex.unlock();
    } catch (...) {
    }
    pred->mutex.unlock();

    if (success){
      return true;
    }else{
      return false;
    }
  };

  bool contains(T element) override{
    bool success = false;
    Node<T> *curr = nullptr;
    size_t key = hash<T>()(element);
    this->head->mutex.lock();
    try{
      curr = this->head;
      while(curr->key < key){
        curr->mutex.unlock();
        curr = curr->next;
        curr->mutex.lock();        
      }
      if (key == curr->key){
        success = true;
      }else{
        throw 505;
      }
    }catch(...){
      success = false;
    }
    curr->mutex.unlock();

    if (success){
      return true;
    }else{
      return false;
    }
    
  };
};