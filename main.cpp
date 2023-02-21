#include <iostream>
#include <string>
#include "./cset/cset.h"

using namespace std;

int main(){
  CoarseSet<string> coarse_grained_set;

  int a,b;
  string c;
  a = 5;
  c = "hello";
  coarse_grained_set.add(c);
  coarse_grained_set.add("world");

  if (coarse_grained_set.contains("hello")){
    cout << "found" << endl;   
  }else{
    cout << "not found" << endl;   
  }
  
  coarse_grained_set.print();
  cout << endl;
  coarse_grained_set.remove(c);
  coarse_grained_set.print();

  return 0;
}
