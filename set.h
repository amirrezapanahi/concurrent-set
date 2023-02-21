#include <iostream>
#include <mutex>
#include "node.h"

template <typename T>
class Set {
  public:
    Node<T>* head;
    std::recursive_mutex mutex;
    virtual bool add(T element)=0;
    virtual bool remove(T element)=0;
    virtual bool contains(T element)=0;
};
