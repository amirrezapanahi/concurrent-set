#include "../set.h"
#include <cstdint>
using namespace std;

template <typename T>
class NonBlockingSet: public Set<T>{

  bool add(T element) override;
  bool remove(T element) override;
  bool contains(T element) override;
};