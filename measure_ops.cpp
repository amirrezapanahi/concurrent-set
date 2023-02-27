#include "./correctness/ctpl_stl.h"
#include "./cset/cset.h"
#include "./fset/fset.h"
#include "./lset/lset.h"
#include "./nbset/nbset.h"
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <typeinfo>
#include <unordered_map>
#include <vector>

using namespace std;

unordered_map<thread::id, atomic<int>[3]> thread_counts;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<float> dis(0.0, 1.0);

bool executing(chrono::time_point<chrono::steady_clock> start_time, int time) {
  return ((chrono::steady_clock::now() - start_time) < chrono::seconds(time));
}

void increment_count(int flag) {
  try {
    if (flag == 0) { // add
      thread_counts[this_thread::get_id()][0]++;
    } else if (flag == 1) { // remove
      thread_counts[this_thread::get_id()][1]++;
    } else if (flag == 2) { // contains
      thread_counts[this_thread::get_id()][2]++;
    }
  } catch (std::out_of_range &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void print_thread_counts() {
  for (auto it = thread_counts.begin(); it != thread_counts.end(); ++it) {
    std::cout << "Thread " << it->first << " made " << it->second[0] << " add calls, " << 
              it->second[1] << " remove calls and " << it->second[2] << " contains calls."
              << std::endl;
  }
}

int* total_operations() {
  static int counts[3];
  for (auto it = thread_counts.begin(); it != thread_counts.end(); ++it) {
    counts[0] = counts[0] + it->second[0];
    counts[1] = counts[1] + it->second[1];
    counts[2] = counts[2] + it->second[2];
  }
  return counts;
}

bool add_op(float operation_selection, float add_probability) {
  return operation_selection >= 0 && operation_selection <= add_probability;
}

bool remove_op(float operation_selection, float add_probability,
               float remove_probability) {
  return operation_selection > add_probability &&
         operation_selection < (add_probability + remove_probability);
}

bool contains_op(float operation_selection, float add_probability,
                 float remove_probability) {
  return operation_selection > (add_probability + remove_probability) &&
         operation_selection < 1;
}

int main(int argc, char *argv[]) {
  // usage: ./main <NUM_THREAD> <IMPLEMENTATION> <TYPE>

  if (argc < 7 || argc > 8) {
    cout << "usage: ./main <%_ADD> <%_REMOVE> <%_CONTAINS> <IMPLEMENTATION> "
            "<TIME> <RANGE> <NUM_THREADS>"
         << endl;
    return 0;
  }

  int num_threads = 0;
  if (argv[7] == nullptr) {
    num_threads = thread::hardware_concurrency();
  } else {
    num_threads = stoi(argv[7]);
  }
  string implementation = argv[4];
  int r = stoi(argv[6]);
  int time = stoi(argv[5]);
  float add_probability = stof(argv[1]);
  float remove_probability = stof(argv[2]);
  float contains_probability = stof(argv[3]);
  uniform_int_distribution<int> keys(1, r);

  if (time < 1 || time > 10) {
    cout << "choose a time period between 1s and 10s" << endl;
    return 0;
  }

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
    cout << "usage: ./main <NUM_THREAD> <IMPLEMENTATION> <TYPE>" << endl;
    cout << "IMPLEMENTATION: \n" << endl;
    cout << "coarse grained set 'cset' \n fine grained set 'fset' \n lazy set "
            "'lset' \n non blocking set 'nbset'"
         << endl;
    return 0;
  }

  // ctpl::thread_pool pool(thread::hardware_concurrency());
  ctpl::thread_pool pool(num_threads);

  auto start_time = std::chrono::steady_clock::now();
  atomic_int count(0);
  float expected_size_set =
      (r * add_probability) / (add_probability + remove_probability);
  atomic<int> total_ops(0);

  while (executing(start_time, time)) {
    float operation_selection = dis(gen);
    int element = keys(gen);
    if (add_op(operation_selection, add_probability)) {
      pool.push([set, element](int) {
        if (set->add(element)) {
          increment_count(0);
        }
      });
      total_ops++;
    } else if (remove_op(operation_selection, add_probability,
                         remove_probability)) {
      pool.push([set, element](int) {
        if (set->remove(element)) {
          increment_count(1);
        };
      });
      total_ops++;
    } else if (contains_op(operation_selection, add_probability,
                           remove_probability)) {
      pool.push([set, element](int) {
        if (set->contains(element)) {
          increment_count(2);
        }
      });
      total_ops++;
    }
  };

  auto end_time = chrono::steady_clock::now();
  auto duration =
      chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

  pool.stop();

  float throughput = total_ops / duration.count();

  print_thread_counts();

  int* ptr = total_operations();
  cout << "~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "Execution time: " << duration.count() << " milliseconds" << endl;
  cout << "Throughput(operation/ms): " << throughput << endl;
  cout << "Actual set size: " << set->size() << endl;
  cout << "Threads made " << ptr[0] << " adds and " << ptr[1]
       << " removes so size: " << (ptr[0] - ptr[1]) << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~" << endl;
  
  return 0;
}
