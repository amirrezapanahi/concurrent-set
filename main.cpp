#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <typeinfo>
#include "./cset/cset.h"
#include "./fset/fset.h"
#include "./lset/lset.h"
#include "./nbset/nbset.h"

using namespace std;

int main(int argc, char*argv[]){
  //usage: ./main <NUM_THREAD> <IMPLEMENTATION> <TYPE>

  if (argc != 4){
    cout << "usage: ./main <NUM_THREAD> <IMPLEMENTATION> <TYPE>" << endl;    
    return 0;
  }
  
  string implementation = argv[2];
  string type = argv[3];
  int num_threads = stoi(argv[1]);
  
  CoarseSet<string> cset;
  FineSet<string> fset;
  // LazySet<string> lset;
  // NonBlockingSet<string> nbset;
  Set<string> *set = nullptr;

  if (implementation.compare("cset") == 0){    
    set = &cset;
  }else if (implementation.compare("fset") == 0){
    set = &fset;
  }else if (implementation.compare("lset") == 0){
    // set = &lset;
  }else if (implementation.compare("nbset") == 0){
    // set = &nbset;
  }else{
    cout << "usage: ./main <NUM_THREAD> <IMPLEMENTATION> <TYPE>" << endl;
    cout << "IMPLEMENTATION: \n" << endl;
    cout << "coarse grained set 'cset' \n fine grained set 'fset' \n lazy set 'lset' \n non blocking set 'nbset'" << endl; 
    return 0;
  }

  const type_info &obj_type = typeid(&set);

  cout << "set: " << obj_type.name() << endl;
  string words[num_threads];
  vector<thread> threads;

  for (int i = 1; i<= num_threads; i++){
    string str = to_string(i);
    words[i-1] = str;
  }

  auto start_time = std::chrono::high_resolution_clock::now();  

  for(int i = 0; i<num_threads; i++){
    string arg = words[i];
    threads.push_back(
      thread([&set, arg](){
        set->add(arg);
      })
    );
  }

  for(int i = 0; i<num_threads; i++){
    threads[i].join();
  }

  for (int i = 0; i<num_threads; i++){
    cout << set->contains(words[i]) << endl;
  }

  auto end_time = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
  cout << "Execution time: " << duration.count() << " milliseconds" << endl;

  return 0;
}
