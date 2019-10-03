## Relevant concepts

- **Stack allocation**.  We have learned in Assignment 0 that there are two
  types of allocated memory: Stack and heap allocated memory. One acquires
  stack allocated memory by using array declarations inside of functions, as
  opposed to global declarations of arrays.

  Memory allocated on the stack is limited in size, but tends to be faster.
  Moreover, stack allocated memory is thread local and therefore provides an
  opportunity to untangle the mutual impact of parallel threads on one another.
  Consequentially, it is an important consideration to employ stack allocated
  memory in the innermost iteration steps, i.e., the Newton iteration for an
  individual point on the complex plane.

  We plan to test this concept by comparing runtimes of variants of our program
  using stack and heap allocated memory in the innermost iteration steps.
  
**Static variables and functions**.

**Initialization of global variables**.

**Working done in thread's local variables --> locality **.

**Dynamic memory**

**Parsing the arguments of the program**.

## Intended program layout
**Overall layout**
-Goal : compute newton iteration of complex numbers and put it in a nice picture
-Main thread:
      - will create other threads
      - allocates data and manages tasks
      - has exit condition? --> has to wait for other threads
      - will neither do computation nor write to file
-Sub threads:
     *Computation threads:
       - assign a range of values, where it will compute the iteration
       	!? how do we allocate the memory? (noncontiguous? --> slides from 02/10/19)
       //future idea: make each thread able to create it's one sub threads


     *Writing threads:
       - writing to file is mostlikly slower than computation, splitting is better
       - only one thread per file for writing, otherwise synchronisation problem
       - maybe a thread for converting numbers to string? --> so far only one thread per file doing both
              ? is converting within one thread faster than converting in another thread and then giving
	      it to a write thread?
       //outlook -> trying to split the writing so we can use multiple threads

**Division in subtasks**
-Main thread:
      - create threads and threadpool
      - give tasks to threads
      - wait for threads to finish
      - shutdown threads once program is done
      ?* benchmark individual tasks --> look into POSIX for function to measure the time
      ? look up thread diagram about how threads are dependent on each other


**Resolution of each subtask**.
t summary of concepts, sketch the intended layout of your program. Here program layout refers to how you intend to split the project task in Assignment 2 into subtasks, and how that devision will be reflected in your implementation