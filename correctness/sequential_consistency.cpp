#include "../cset/cset.h"
#include "../fset/fset.h"
#include "../lset/lset.h"
#include "../nbset/nbset.h"
// #include "../seqset.h"
#include "ctpl_stl.h"
#include <iostream>
#include <thread>

using namespace std;

void add1(Set<int> *set) {
  set->add(1);
  printf("add(1) completed by thread - %d \n", this_thread::get_id());
}

void add2(Set<int> *set) {
  set->add(2);
  printf("add(2) completed by thread - %d \n", this_thread::get_id());
}

void contains1(Set<int> *set) {
  set->contains(1);
  printf("contains(1) completed by thread - %d \n", this_thread::get_id());
}

void contains2(Set<int> *set) {
  set->contains(2);
  printf("contains(2) completed by thread - %d \n", this_thread::get_id());
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "give implementation type as arguent" << endl;
    return 0;
  }

  string implementation = argv[1];

  CoarseSet<int> cset;
  FineSet<int> fset;
  LazySet<int> lset;
  // NonBlockingSet<string> nbset;
  Set<int> *set = nullptr;

  if (implementation.compare("cset") == 0) {
    set = &cset;
  } else if (implementation.compare("fset") == 0) {
    set = &fset;
  } else if (implementation.compare("lset") == 0) {
    set = &lset;
  } else if (implementation.compare("nbset") == 0) {
    // set = &nbset;
  } else {
    cout << "IMPLEMENTATION: \n" << endl;
    cout << "coarse grained set 'cset' \n fine grained set 'fset' \n lazy set "
            "'lset' \n non blocking set 'nbset'"
         << endl;
    return 0;
  }

  ctpl::thread_pool pool(3);
  pool.push([set](int) {
    set->add(1);
  });

  pool.push([set](int) {
    set->add(2);
  });

  pool.push([set](int) {
    set->remove(1);
  });

  pool.push([set](int) {
    set->contains(1);
  });

  pool.push([set](int) {
    set->contains(2);
  });
  return 0;
}