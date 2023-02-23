#include "../set.h"
#include <cstdint>
using namespace std;

template <typename T>
class CoarseSet : public Set<T>{
  public:
    CoarseSet(){
      this->head = new Node<T>();
      this->head->key = 0;
      this->head->next = new Node<T>();
      this->head->next->key = SIZE_MAX;
    }
    
    ~CoarseSet(){
      Node<T>* current = this->head;
      while (current != NULL){
        Node<T> *next = current->next;
        delete current;
        current = next;
      }
    }
    
    bool add(T element) override{
        bool success = false;
        Node<T> *pred, *curr;
        size_t key = hash<T>()(element);
        this->mutex.lock(); //activate mutex lock
        try{
            pred = this->head;
            curr = pred->next;
            while (curr->key < key){
                pred = curr;
                curr = curr->next;
            }
            if (key == curr->key){
                throw(true);
            }else{
                Node<T> *node = new Node<T>(element);
                node->next = curr;
                pred->next = node;
                success = true;
            }
        }catch (bool duplicate){
            cout << "Add operation unsuccessful:" << endl;
            cout << "- Duplicate element" << endl;
            success = false;
        }
        this->mutex.unlock(); //unlock mutex so other threads can now access
        
        if (success){
            return true;
        }else{
            return false;
        }
    }

    bool remove(T element) override{
    
        bool success = false;
        Node<T> *pred, *curr;
        size_t key = hash<T>()(element);
        
        this->mutex.lock();
        try{
            pred = this->head;
            curr = pred->next;
            while (curr->key < key){
                pred = curr;
                curr = curr->next;
            }    
            if (key == curr->key){
                pred->next = curr->next;
                success = true;
            }else{
                throw(true);
            }
        }catch (bool nonexistent){            
            cout << "Remove operation unsuccessful:" << endl;
            cout << "- Element doesn't exist" << endl;
            success = false;
        }
        this->mutex.unlock();

        if (success){
            return true;
        }else{
            return false;
        }
    }

    bool contains(T element) override{
        bool success = false;
        Node<T> *curr;
        size_t key = hash<T>()(element);

        this->mutex.lock();
        try{   
            curr = this->head;
            while (curr->key < key){
                curr = curr->next;
            }        
            if (key == curr->key){
                success = true;
            }else{
                throw(true);
            }
        }catch (bool nonexistent){            
            cout << "Contain operation unsuccessful:" << endl;
            cout << "- Element does not exist" << endl;
            success = false;
        }
        this->mutex.unlock();

        if (success){
            return true;
        }else{
            return false;
        }
    }

    void print(){
        Node<T>* current = this->head;
        while (current != NULL){
            cout << "node: " << current->item << " with key -> " << current->key << endl;
            Node<T> *next = current->next;
            current = next;
        }
    }
};