#include "../cset/cset.h"
#include "../fset/fset.h"
#include "../lset/lset.h"
#include "../nbset/nbset.h"
#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

void test_add_int() {
  CoarseSet<int> cset;
  int a = 400;
  cset.add(3);
  cset.add(a);
  assert(cset.contains(3));
  assert(cset.contains(a));
  cout << "test_add_int: passed" << endl;
}

void test_remove_empty_set(Set<string> &customSet) {
  Set<string> *set = &customSet;
  assert((set->remove("hello") == false));
  cout << "test_remove_empty_set: passed" << endl;
}

void test_add_string(Set<string> &customSet) {
  Set<string> *set = &customSet;
  string a = "mellow";
  set->add("hello");
  set->add(a);
  assert(set->contains("hello"));
  assert(set->contains(a));
  cout << "test_add_string: passed" << endl;
}

void test_remove(Set<string> &customSet) {
  Set<string> *set = &customSet;
  string a = "mellow";
  set->add("hello");
  set->add(a);
  assert(set->contains("hello"));
  assert(set->contains(a));
  set->remove(a);
  assert((set->contains(a) == false));
  cout << "test_remove: passed" << endl;
}

void test_duplicate_entry(Set<string> &customSet) {
  Set<string> *set = &customSet;
  set->add("dog");
  assert((set->add("dog") == false));
  cout << "test_duplicate_entry: passed" << endl;
}

void test_add_5_elements(Set<string> &customSet) {
  Set<string> *set = &customSet;
  string a = "dog";
  string b = "adhkjaskjafkjadhkjasdkjashakjjkn";
  string c = "ajdkasladhjladjdjdjjdjddahgfhjlkdashfjlkajfklfjjk";
  string d = "jdfadafhjjkfjasdlfsdbkjabchksbvabcdaf";
  string e = "adajbcabckadhjadhabca";

  set->add(a);
  set->add(b);
  set->add(c);
  set->add(d);
  set->add(e);

  assert(set->contains(a));
  assert(set->contains(b));
  assert(set->contains(c));
  assert(set->contains(d));
  assert(set->contains(e));

  cout << "test_add_5_elements: passed" << endl;
}

int main(int argc, char *argv[]) {

  CoarseSet<string> cset;
  FineSet<string> fset;
  LazySet<string> lset;
  NonBlockingSet<string> nbset;
  Set<string> *set = nullptr;

  string implementation = argv[1];

  if (implementation.compare("cset") == 0) {
    set = &cset;
  } else if (implementation.compare("fset") == 0) {
    set = &fset;
  } else if (implementation.compare("lset") == 0) {
    set = &lset;
  } else if (implementation.compare("nbset") == 0) {
    set = &nbset;
  } else {
    cout << "usage: ./main <NUM_THREAD> <IMPLEMENTATION> <TYPE>" << endl;
    cout << "IMPLEMENTATION: \n" << endl;
    cout << "coarse grained set 'cset' \n fine grained set 'fset' \n lazy set "
            "'lset' \n non blocking set 'nbset'"
         << endl;
    return 0;
  }

  // test_add_int();
  test_remove_empty_set(*set);
  test_add_string(*set);
  test_remove(*set);
  test_duplicate_entry(*set);
  test_add_5_elements(*set);

  return 0;
}