#include <atomic>

template <typename T>
class CSet{
    private:
    Node head;
    public:
    bool contains (T element);
    bool add (T element); 
    bool remove (T element); 
}

template <typename T>
bool CSet<T>::contains (T element){
    /* return true if present */
}

template <typename T>
bool CSet<T>::add (T element){
    /* return true if was not present before */
}

template <typename T>
bool CSet<T>::remove (T element){
    /* return true if was present before */  
} 