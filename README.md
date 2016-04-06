#(sea)-world

##Memory Management and Garbage Collection in C
Memory Manager replaces the functions `malloc` and `free` with improved versions `myalloc` and `myfree`.  The Garbage Collection determines which blocks in the heap are inaccessible from the stack, and then deallocates those blocks.  It also coalesces the unallocated blocks after it finishes.

To use the code, you need to compile it with a test file.
`cc memorymgr.c memorymgrtest.c -o memorymgrtest`
`cc memorymgr.c gc.c gctest.c -o gctest`

Garbage Collection only works for Linux.