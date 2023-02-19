#include <iostream>
#include <mutex>
#include "node.h"

template <typename T>
class Set {
  public:
    Node<T> head;
    std::mutex lock;
    virtual bool add(T item);
    virtual bool remove(T item);
    virtual bool contains(T item);
};