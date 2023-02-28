#include "./correctness/ctpl_stl.h"
#include "./cset/cset.h"
#include "./fset/fset.h"
#include "./lset/lset.h"
#include "./nbset/nbset.h"
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <typeinfo>
#include <unordered_map>
#include <vector>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<float> dis(0.0, 1.0);

bool executing(chrono::time_point<chrono::steady_clock> start_time, int time) {
  return ((chrono::steady_clock::now() - start_time) < chrono::seconds(time));
}

void increment_count(int flag,
                     unordered_map<thread::id, atomic<int>[3]> &thread_counts) {
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

void print_thread_counts(
    unordered_map<thread::id, atomic<int>[3]> &thread_counts) {
  for (auto it = thread_counts.begin(); it != thread_counts.end(); ++it) {
    std::cout << "Thread " << it->first << " made " << it->second[0]
              << " add calls, " << it->second[1] << " remove calls and "
              << it->second[2] << " contains calls." << std::endl;
  }
}

int *thread_operations(
    unordered_map<thread::id, atomic<int>[3]> &thread_counts) {
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

vector<thread::id> check_starvation(int *total, unordered_map<thread::id, atomic<int>[3]> &thread_counts){
  vector<thread::id> starved_threads;

  float add_average = (float)total[0] / thread_counts.size();
  float remove_average = (float)total[1] / thread_counts.size();
  float contains_average = (float)total[2] / thread_counts.size();
  float total_average = add_average + remove_average + contains_average;

  for (auto it = thread_counts.begin(); it != thread_counts.end(); ++it) {
    float total_operations = it->second[0] + it->second[1] + it->second[2];
    if (total_operations < (0.1 * total_average)){
      starved_threads.push_back(it->first);
    }
  }

  return starved_threads;
}

void write_csv(std::string filename,
               std::vector<std::pair<std::string, std::vector<int>>> dataset) {

  // Create an output filestream object
  std::ofstream myFile(filename);

  // Send column names to the stream
  for (int j = 0; j < dataset.size(); ++j) {
    myFile << dataset.at(j).first;
    if (j != dataset.size() - 1)
      myFile << ","; // No comma at end of line
  }
  myFile << "\n";

  // Send data to the stream
  for (int i = 0; i < dataset.at(0).second.size(); ++i) {
    for (int j = 0; j < dataset.size(); ++j) {
      myFile << dataset.at(j).second.at(i);
      if (j != dataset.size() - 1)
        myFile << ","; // No comma at end of line
    }
    myFile << "\n";
  }

  // Close the file
  myFile.close();
}

int main(int argc, char *argv[]) {
  // usage: ./main <NUM_THREAD> <IMPLEMENTATION> <TYPE>

  if (argc < 7) {
    cout << "usage: ./main <%_ADD> <%_REMOVE> <%_CONTAINS> <IMPLEMENTATION> "
            "<RANGE> <NUM_THREADS>"
         << endl;
    return 0;
  }

  int num_threads = 0;
  if (argv[6] == nullptr) {
    num_threads = thread::hardware_concurrency();
  } else {
    num_threads = stoi(argv[6]);
  }
  string implementation = argv[4];
  int r = stoi(argv[5]);
  float add_probability = stof(argv[1]);
  float remove_probability = stof(argv[2]);
  float contains_probability = stof(argv[3]);
  uniform_int_distribution<int> keys(1, r);

  // if (time < 1 || time > 10) {
  //   cout << "choose a time period between 1s and 10s" << endl;
  //   return 0;
  // }

  CoarseSet<int> cset;
  FineSet<int> fset;
  LazySet<int> lset;
  NonBlockingSet<int> nbset;
  Set<int> *set = nullptr;

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

  vector<pair<string, vector<int>>> throughputs;
  int buffer = 0;
  vector<int> thread_numbers = {1, 2, 4, 8, 16, 32, 48, 64, 80, 96};
  float expected_size_set =
      (r * add_probability) / (add_probability + remove_probability);

  //prefill

  while (set->size()<expected_size_set){
    //sequentially add to set until full
    int element = keys(gen);
    set->add(element);
  }

  atomic<int> total_operations(set->size());
  // for (int thread = 1; thread <= thread_numbers.size(); thread++) {
    // string colname = "Thread" + to_string(thread_numbers[thread - 1]);
    // vector<int> recorded_throughputs;
    // repeat experiment 5 times
    // for (int i = 0; i < 5; i++) {
      // ctpl::thread_pool pool(thread_numbers[thread - 1]);
      ctpl::thread_pool pool(num_threads);

      unordered_map<thread::id, atomic<int>[3]> thread_counts;
      auto start_time = std::chrono::steady_clock::now();

      total_operations.store(0);
      while (executing(start_time, 5)) {
        float operation_selection = dis(gen);
        int element = keys(gen);
        if (add_op(operation_selection, add_probability)) {
          pool.push([set, element, &total_operations, &thread_counts](int) {
            if (set->add(element)) {
              increment_count(0, thread_counts);
            }
            total_operations.fetch_add(1);
          });
        } else if (remove_op(operation_selection, add_probability,
                             remove_probability)) {
          pool.push([set, element, &total_operations, &thread_counts](int) {
            if (set->remove(element)) {
              increment_count(1, thread_counts);
            };
            total_operations.fetch_add(1);
          });
        } else if (contains_op(operation_selection, add_probability,
                               remove_probability)) {
          pool.push([set, element, &total_operations, &thread_counts](int) {
            if (set->contains(element)) {
              increment_count(2, thread_counts);
            }
            total_operations.fetch_add(1);
          });
        }
      };

      auto end_time = chrono::steady_clock::now();
      auto duration =
          chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

      pool.stop();

      int *ptr = thread_operations(thread_counts);
      vector<thread::id> starved_threads = check_starvation(ptr, thread_counts);
      int total_ops = (ptr[0] + ptr[1] + ptr[2]);
      float throughput = (total_operations) / duration.count();

      print_thread_counts(thread_counts);

      cout << "~~~~~~~~~~~~~~~~~~~~~~" << endl;
      cout << "Execution time: " << duration.count() << " milliseconds" << endl;
      cout << "Total operation: " << total_operations << endl;
      cout << "Throughput(operation/ms): " << throughput << endl;
      cout << "Actual set size: " << set->size() << endl;
      cout << "Threads made " << ptr[0] << " adds and " << ptr[1]
           << " removes so size: " << (expected_size_set + (ptr[0] - ptr[1])) << endl;
      cout << "Starved threads: " << endl;
      for (int j = 0; j < starved_threads.size(); j++){
        cout << " - Thread " << starved_threads[j] << endl;
      }
      cout << "~~~~~~~~~~~~~~~~~~~~~~" << endl;

      // recorded_throughputs.push_back(throughput);
    // };

    // throughputs.push_back(
    //     pair<string, vector<int>>(colname, recorded_throughputs));
    // string filename = "./raw_results/" + implementation + "_" +
    //                   to_string(add_probability) + "_" +
    //                   to_string(remove_probability) + "_" +
    //                   to_string(contains_probability) + "_" + "raw.csv";
    // write_csv(filename, throughputs);
  // };

  return 0;
}
