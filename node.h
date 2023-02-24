#ifndef NODE_H
#define NODE_H
#include <stddef.h>
#include <iostream>
#include <mutex>

template <typename T>
class Node{
    public:
        T item;
        std::size_t key;
        Node* next;
        std::recursive_mutex mutex;
        Node() = default;
        Node(T item){
            this->item = item;
            this->key = std::hash<T>()(item);
            this->next = nullptr;
        }
        void printNode(){
            std::cout << "item: " << this->item << std::endl;
            std::cout << "key: " << this->key << std::endl;
            std::cout << "next: " << this->next << std::endl;
        }
};
#endif