## Relevant concepts

**Stack allocation**.  We have learned in Assignment 0 that there are two
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

**Dynamic memory and memory fragmentation**
  Fragmentation can occur when we allocate dynamic memory in noncontiguous blocks.
  The memory "between" the blocks is unallocated and can be too small to be usable.
  These gaps summarize leaving substantial amounts of memory wasted. In addition to
  that the noncontiguous allocated memory can slow down memory access.
  When dynamically allocating memory it is imperative to free the memory, once
  the process is finished, in order to avoid wasting memory space (memory leakage). 
  
**Reading and writing files**
  Files can be read with fread() and written with fwrite(). These functions are
  less complex than fscanf() or fprint() and therefore faster. fwrite() and fread()
  are using buffer to write or read. This results in faster computation.
  
**Command line arguments**
  It is possible to pass arguments to a program via command line. If intended, these
  additional inputs allow the user to change parameters or program behaviour.
  POSIX library contains an easy way to implement the parsing of command line arguments.

**Locality**
  According to wikipedia locality is defined as:
  >the tendency of a processor to access the same set of memory locations reptitively over
  > a short period of time
  >*-[wikipedia:Locality of reference]*
							      
  Exploiting this principle we can use this to access arrays in a faster way, increasing performance.
  Most arrays can be put within one cache line. Caching reduces direct access to memory and keeps
  part of the data in one quickly accessible location.
  It is important to remember, that accessing data from different parts of the memory
  will always take more time than accessing data from cache.

  In the assignments we looked at techniques like loop unrolling to optimize performance. 

**Alignment and Padding**
  Different types of variables require different amounts of memory. Variables should be declared
  in groups sorted by type. This way the compiler can allocate memory more efficient. This avoids
  waste of memory space.

**HDD vs SSD**
  While a single SSD is generally faster than an HDD gantenbein uses one SSD and a HDD-raid.
  As seen in the tests of Assignment 1, this raid increases performance of the HDDs leading
  to a similar performace as the SSD. Therefore the operation for writing files should not have
  a big overhead.

**Threads and Mutexes**
  -Synchronization
  -Transfering Data

**Working done in thread's local variables**

**Load Balancing**

**Possible Bottlenecks?**

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


    **Writing threads:
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