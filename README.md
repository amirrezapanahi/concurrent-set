# How to run 

1. To run unit tests 

```$ cd tests```

```$ g++ tests.cpp -o tests```

```$ ./tests <IMPLEMENTATION_NAME>```

where implementation_name is either 'cset', 'fset', 'lset' or 'nbset' 

2. To run sequential consistency model

``$ cd correctness``

``$ g++ sequential_consistency.cpp -o seqcons``

``$ ./seqcons <IMPLEMENTATION_NAME>``

3. To run benchamrking, number of operations in a fixed 5 second time interval

``$ g++ measure_ops.cpp -o measure_ops``

``$ ./measure_ops <%_ADD> <%_REMOVE> <%_CONTAINS> <IMPLEMENTATION> <RANGE> <NUM_THREADS>``

