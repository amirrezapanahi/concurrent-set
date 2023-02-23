#ifndef SET_H
#define SET_H
#include <iostream>
#include <mutex>
#include "node.h"

template <typename T>
class Set {
  protected:  
    Node<T>* head;
    std::recursive_mutex mutex;
  public:
    virtual bool add(T element)=0;
    virtual bool remove(T element)=0;
    virtual bool contains(T element)=0;
};
#endif
