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

void test_duplicate_entry(){
  CoarseSet<string> cset;
  cset.add("dog");
  assert((cset.add("dog") == false));
  cout << "test_duplicate_entry: passed" << endl;
}

void test_add_5_elements(){
  CoarseSet<string> cset;
  string a = "dog";
  string b = "adhkjaskjafkjadhkjasdkjashakjjkn";
  string c = "ajdkasladhjladjdjdjjdjddahgfhjlkdashfjlkajfklfjjk";
  string d = "jdfadafhjjkfjasdlfsdbkjabchksbvabcdaf";
  string e = "adajbcabckadhjadhabca";

  cset.add(a);
  cset.add(b);
  cset.add(c);
  cset.add(d);
  cset.add(e);

  assert(cset.contains(a));
  assert(cset.contains(b));
  assert(cset.contains(c));
  assert(cset.contains(d));
  assert(cset.contains(e));

  cout << "test_add_5_elements: passed" << endl;
  
}

int main(){
  
  test_add_int();
  test_add_string();
  test_remove();
  test_remove_empty_set();
  test_duplicate_entry();
  test_add_5_elements();

  return 0;
}