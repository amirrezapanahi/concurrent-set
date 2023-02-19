#include <stddef.h>
#include <iostream>
#include "node.h"

template <typename T>
Node<T>::Node(T item) {
  this->item = item;
  this->key = std::hash<T>(item);
  this->next = nullptr;
}

template <typename T>
void Node<T>::printNode(){
  std::cout << "item: " << this->item << std::endl;
  std::cout << "key: " << this->key << std::endl;
  std::cout << "next: " << this->next << std::endl;
} 