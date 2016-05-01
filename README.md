#(sea)-world

##Memory Management and Garbage Collection in C
Memory Manager replaces the functions `malloc` and `free` with improved versions `myalloc` and `myfree`.  The Garbage Collection determines which blocks in the heap are inaccessible from the stack, and then deallocates those blocks.  It also coalesces the unallocated blocks after it finishes.

To use the code, you need to compile it with a test file.
1. `cc memorymgr.c memorymgrtest.c -o memorymgrtest`
2. `cc memorymgr.c gc.c gctest.c -o gctest`

_Garbage Collection only works for Linux_.

##Pattern Search using Threads
Pattern search finds the instance of a pattern string inside a text string.  Instead of having one thread process the entire array, multiple threads work together, with each thread only having to search part of it.