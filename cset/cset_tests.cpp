#include <assert.h>
#include <iostream>
#include <string>
#include "cset.h"
using namespace std;

void test_add_int(){
  CoarseSet<int> cset;
  int a = 400;
  cset.add(3);
  cset.add(a);
  assert(cset.contains(3));
  assert(cset.contains(a));
  cout << "test_add_int: passed" << endl;
}

void test_add_string(){
  CoarseSet<string> cset;
  string a = "mellow";
  cset.add("hello");
  cset.add(a);
  assert(cset.contains("hello"));
  assert(cset.contains(a));
  cout << "test_add_string: passed" << endl;
}

void test_remove(){
  CoarseSet<string> cset;
  string a = "mellow";
  cset.add("hello");
  cset.add(a);
  assert(cset.contains("hello"));
  assert(cset.contains(a));
  cset.remove(a);
  assert((cset.contains(a) == false));
  cout << "test_remove: passed" << endl;
}

void test_remove_empty_set(){
  CoarseSet<string> cset;
  assert((cset.remove("hello") == false));
  cout << "test_remove_empty_set: passed" << endl;
}

int main(){
  
  test_add_int();
  test_add_string();
  test_remove();
  test_remove_empty_set();

  return 0;
}