#ifndef NODE_H
#define NODE_H
#include <stddef.h>
#include <iostream>
#include <mutex>
#include <atomic>
#include "amr.cpp"

using namespace std;

template <typename T>
class Node{
    public:
        T item;
        size_t key;
        Node* next;
        AtomicMarkableReference* atmc_next;
        recursive_mutex mutex;
        bool marked;
        Node() = default;
        Node(T item){
            this->item = item;
            this->key = std::hash<T>()(item);
            this->next = nullptr;
            this->atmc_next = nullptr;
            this->marked = false;
        }
        void printNode(){
            std::cout << "item: " << this->item << std::endl;
            std::cout << "key: " << this->key << std::endl;
            std::cout << "next: " << this->next << std::endl;
        }
};
#endif