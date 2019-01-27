A constantly evolving lisp interpreter in Object-Oriented C++

Requirements : GCC w/ c++14

To run with library:
   cat lib.txt - | ./stanley

Credits to Professor Norman Ramsey for his great contribution
through his class 'Programming Languages' at Tufts University.

Completed:
          - recursion
          - lists
          - functions/lambdas as first class citizens
          - primitives (if, -, nil?, cons, car, cdr)
          - lists hold expressions -- can be used for delayed evaluation by `car`
          - begin & let library functions based on list evaluation
          
In progress:
          - lambda variable capture
          - limiting scope of `val`
          - garbage collection/management through smart pointers of heap
          - type system
