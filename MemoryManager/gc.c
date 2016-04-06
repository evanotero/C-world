/**
 * author Evan Otero
 * March 18 2016
 * Garbage Collection for C
 */

#include <stdio.h>
#include "gc.h"
#include "memorymgr.h"

void *stackBottom();
void mark(int *p);
void sweep(int *p);

// Helper Functions
int *isPtr(int *p);
int isMarked(int *p);
void markBlock(int *p);
void unmarkBlock(int *p);

// perform garbage collection on the heap
void gc() {
    int **max = (int **) stackBottom();
    int *q;
    int **p = &q;
    while (p < max) {
        mark(*p);
        p++;
    }
    sweep(q);
    coalesce();
}

// Find the absolute bottom of the stack.
void *stackBottom() {
  unsigned long bottom;
  FILE *statfp = fopen("/proc/self/stat", "r");
  fscanf(statfp,
    "%*d %*s %*c %*d %*d %*d %*d %*d %*u "
    "%*u %*u %*u %*u %*u %*u %*d %*d "
    "%*d %*d %*d %*d %*u %*u %*d "
    "%*u %*u %*u %lu", &bottom);
  fclose(statfp);
  return (void *) bottom;
}

void mark(int *p) {
    int i = 0, size;
    int *ptr;
    ptr = isPtr(p);
    if (ptr != NULL && isAllocated(ptr)) {
        if (!isMarked(ptr)) {
            markBlock(ptr);
        }
        size = length(ptr) / 4;
        while (++i < size) {
            mark(ptr + i);
        }
    }
}

void sweep(int *ptr) {
    ptr = firstBlock();
    while (ptr != lastBlock()) {
        if (isAllocated(ptr) && !isMarked(ptr))
            myfree(ptr);
        else if (isMarked(ptr))
            unmarkBlock(ptr);
        ptr = nextBlock(ptr);
    }
}

int *isPtr(int *p) {
    if (firstBlock() <= p && p <= lastBlock())
        return p;
    else
        return NULL;
}

int isMarked(int *p) {
    return *(p - 1) % 4 == 3;
}

void markBlock(int *p) {
    *(p - 1) += 2;
}

void unmarkBlock(int *p) {
    *(p - 1) -= 2;
}