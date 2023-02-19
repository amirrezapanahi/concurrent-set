#include <atomic>
#include "set.h"

template <typename T>
class CoarseSet: public Set<T>{
    public:
    
    CoarseSet(){        
        this->head = nullptr; 
    }
    
    ~CoarseSet(){
        while (this->head != NULL){
            Node<T> *tmpNode = this->head;
            delete this->head;
            this->head = tmpNode;
        }
    }
    
    bool contains (T element) override{
        bool success = false;
        Node<T> curr;
        size_t key = std::hash<T>(element);

        this->lock.lock();
        try{   
        curr = this->head;
        while (curr.key < key){
            curr = curr.next;
        }        
        if (key == curr.key){
            success = true;
        }else{
            throw(true);
        }
        }catch (bool nonexistent){            
            std::cout << "Add operation unsuccessful:" << std::endl;
            std::cout << "- Duplicate element" << std::endl;
            success = false;
        }
        this->lock.unlock();

        if (success){
            return true;
        }else{
            return false;
        }
        
    }
    
    bool add (T element) override{
        bool success = false;
        Node<T> pred, curr;
        size_t key = std::hash<T>(element);

        this->lock.lock(); //activate mutex lock
        try{
            pred = this->head;
            curr = pred.next;
            while (curr.key < key){
                pred = curr;
                curr = curr.next;
            }
            if (key == curr.key){
                throw(true);
            }else{
                Node<T> node = new Node<T>(element);
                node.next = curr;
                pred.next = node;
                success = true;
            }
        }catch (bool duplicate){
            std::cout << "Add operation unsuccessful:" << std::endl;
            std::cout << "- Duplicate element" << std::endl;
            success = false;
        }
        this->lock.unlock(); //unlock mutex so other threads can now access
        
        if (success){
            return true;
        }else{
            return false;
        }
    } 
    
    bool remove (T element) override{
        bool success = false;
        Node<T> pred, curr;
        size_t key = std::hash<T>(element);
        
        this->lock.lock();
        try{
            pred = this->head;
            curr = pred.next;
            while (curr.key < key){
                pred = curr;
                curr = curr.next;
            }    
            if (key == curr.key){
                pred.next = curr.next;
                success = true;
            }else{
                throw(true);
            }
        }catch (bool nonexistent){            
            std::cout << "Remove operation unsuccessful:" << std::endl;
            std::cout << "- Element doesn't exist" << std::endl;
            success = false;
        }
        this->lock.unlock();

        if (success){
            return true;
        }else{
            return false;
        }
        
    }
};

