## Assignment 3 report

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
  additional inputs allow the user to change the program behaviour.
  POSIX library contains an easy way to implement the parsing of command line arguments.

**Locality**
  According to wikipedia locality is defined as:
  
> the tendency of a processor to access the same set of memory locations repetitively over a short period of time
> *-[wikipedia: Locality of reference]*
							      
  Exploiting this principle we can use this to access arrays in a faster way, increasing performance.
  Most arrays, or a big portion of them, can be put within one cache line. Caching reduces direct access to memory and keeps
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

**OpenMP**
>OpenMP is an application programming interface (API) to support the shared-memory multiple-thread form of parallel application development. 
> *T.Sterling, M.Anderson, M. Brodowicz (2018) High Performance Computing, Modern Systems and Practices.

 OpenMP is suitable for fine-grained parallelism. Compared to POSIX threads it provides the possibility of a much shorter and more decriptive code implementation.
 
 Threads are the primary means for parallel computation. Each thread is a schedulable entity within which a sequence of instructions are executed, it has private variables and internal control at its disposal. OpenMP provides the flexibility to define the type of scheduling the programmer prefers. This is done in an efficient and concise code. Specified pragmas are used to define the characteristics of the parallelization tailored for a specific proram. Furthermore, offloading can be used as a simple way to exploit co-processors. On each device there could be a team of threds adjusted to execute a specific type of problem. The collection of the teams of threads is a league. For instance, a for loop can be split over a number of teams, together constituting a league that is executing the whole loop in parallel.

## Intended program layout
**Overall layout**
The main goal of assignment 3 is to compute and count distances between points in 3-dimensional space, using OpenMP as instructed in the problem's implementation details on the course website.
The output of the program will be printed on the command terminal, which will consist of two column, the first being the distance between the two 3-dimational points and the second the number of times this distance was computed among all distances.

* Input
 The points in the 3-dimensional space are given in a file. To read this file we have to analyze it with
 the help of fread(), fseek() and so on. The file is read into a buffer. The chars in the buffer are
 are then converted into integers. To make sure we do not lose information, since the coordinates
 are given as float, we multiply the numbers by 1000. This should not be forgotten to make sure the
 results are of correct size.


** Division of Subtasks **

* Read and parsing the file

* Computation of the distances and increment of the corresponding counts
 Use Pointers for better performance exploiting OmpenMP this way
 Babylonial method for the square root turns out to be slower so we used the sqrt() function.
 We use the flots as the data tyoe for the program

* Sorting
 To sort the results we are using an ordered linked list. The list is getting sorted everytime a new
 value is added.

* Output
 For printing the results we are using fprintf() and post the result to the stdout. Manipulating the
 string while printing can be useful to avoid a conversion from int to float. Since this counts more
 as a "quick-and-dirty" solution it should be properly discussed.



* Memory management
Sorting
 Linked list


* Parallelization
 OpenML





**Resolution of each subtask**.

**Implementation Details**

The programm at no time may consume more than 1 GiBi byte = 1024^3 bytes of memory

You may not make any assumption on the number of cells except that there is less than 2^32.
