# Notes for report
----

1. using abstract class 'set' to act as an interface for the 3 implementations since they
will all have the same functions 
2. using templates for each function for performance since any unused function templates will
not compile saving space complexity
3. for theoretical arguments:
  - list premises (hard truths) like "mutexes ensure that there are no race conditions"
  - show how premises come to a certain conclusion
  - mention "invariants"
4. for each test run through the data structure to ensure that everything is in the right place
and that there was no data corruption
5. decided to put function definitions for each set implementation in header files as not necessary to
abstract definitions into seperate cpp file given that we only need the 3 fixed functions for this practical

correctness conditions:
- Linearizability
- Sequential consistency
- Quiescent consistency
- Lock-freedom
- Wait-freedom
- Obstruction-freedom
- Progress guarantee 
- Abscence of data races
- Atomicity

theoretical arguments:
https://www.youtube.com/watch?v=5eGsdrdeBQ8