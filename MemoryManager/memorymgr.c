/**
 * author Evan Otero
 * March 18 2016
 * Memory manager for C
 */

#include <stdlib.h>
#include <stdio.h>
#include "memorymgr.h"

void allocateBlock(int *p);
void unallocateBlock(int *p);
int length(int *p);

int *start;
int *last;
int sizeHeap;

// initialize a simulated heap
void initmemory(int size) {
    int blocklen = ((size + 3) / 8 + 1) * 8;
    sizeHeap = blocklen;
    start = (int *) malloc(sizeHeap + 8);
    if (start == NULL) {
        printf("Cannot initialize that much space!\n");
        exit(1);
    }
    start++; // Skip over 4 bytes
    *start = blocklen; // Header value
    start++; // Place start at large free block
    last = nextBlock(start); // Place last at Sentinal Block header
    *last = 1; // Sentinel Block header value
}

// allocate a block of length (or more) bytes
void *myalloc(int length) {
    int blockLen = ((length + 3) / 8 + 1) * 8;
    int *temp = firstBlock(), oldSize = *(firstBlock() - 1), newSize = 0;
    while (temp != lastBlock()) {
        if (!isAllocated(temp) && oldSize >= blockLen) {
            *(temp - 1) = blockLen;
            allocateBlock(temp);
            newSize = oldSize - blockLen;
            if (newSize > 0)
                *(nextBlock(temp) - 1) = newSize;
            return temp;
        }
        temp = nextBlock(temp);
        oldSize = *(temp - 1);
    }
    return NULL;
}

// free an allocated block
void myfree(void *ptr) {
    unallocateBlock(ptr);
}

// go through the heap, coalescing unallocated blocks
void coalesce() {
    int *ptr = firstBlock(), *nextptr;
    while (ptr != lastBlock()) {
        nextptr = nextBlock(ptr);
        if (isAllocated(ptr) || isAllocated(nextptr) || *(nextptr - 1) == 0) {
            ptr = nextBlock(ptr);
            continue;
        }
        *(ptr - 1) += length(nextptr);
    }
}

// print info about the simulated heap, for debugging purposes
void printallocation() {
    int count = 0, *temp = firstBlock();
    char *allocated;
    while (temp != lastBlock()) {
        switch (isAllocated(temp)) {
            case 0:
                allocated="unallocated";
                break;
            case 1:
                allocated="allocated";
                break;
        }
        printf("Block: %d  Size: %d \t%s\n", count, length(temp), allocated);
        temp = nextBlock(temp);
        count++;
    }
    printf("\n"); // Blank line after heap info
}

/* Helper Functions */

// is the block at location p allocated?
int  isAllocated(int *p) {
    return *(p - 1) % 2 == 1;
}

// return a pointer to the block that follows p
int *nextBlock(int *p) {
    return (p + length(p) / 4);
}

// return a pointer to the first block on the heap
int *firstBlock() {
    return start;
}

// return a pointer to the sentinel block
int *lastBlock() {
    return last;
}

// return length of the block
int length(int *p) {
    return *(p - 1) / 8 * 8;
}

// Allocate a block
void allocateBlock(int *p) {
    *(p - 1) += 1;
}

// Unallocate a block
void unallocateBlock(int *p) {
    *(p - 1) -= 1;
}
