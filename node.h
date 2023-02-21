#include <stddef.h>
template <typename T>
class Node{
    public:
        T item;
        std::size_t key;
        Node* next;
        Node() = default;
        Node(T item){
            this->item = item;
            this->key = std::hash<T>{}(item);
            this->next = nullptr;
        }
        void printNode(){
            std::cout << "item: " << this->item << std::endl;
            std::cout << "key: " << this->key << std::endl;
            std::cout << "next: " << this->next << std::endl;
        }
};