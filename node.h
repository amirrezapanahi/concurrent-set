#include <stddef.h>
template <typename T>
class Node{
    public:
        T item;
        size_t key;
        Node next;
        Node(T item);
        void printNode();
};