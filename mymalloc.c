#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

void Error(enum _errors err, int line, char *file) {
  printf("Error at line: %d in %s:\n", line, file);

  switch (err) {
    case NO_MEM: printf("No memory available for allocation\n");
    case SHORT_MEM: printf("Not enough memory available for allocation\n");
    case BIG: printf("Size requested larger than heap size\n");
  }
}

/* declared only once so just inline */
inline void set_up(size_t size) {
  /* Make size a multiple of 2 */
  size = (size & 0x1) ? size + 1 : size;

  /* equals 5000 together for size, no 1 bit at end because free*/
  if (!_initialized_) {
    assign_16(myblock, 5000);
    /*
    myblock[0] = 0x13;
    myblock[1] = 0x88;
     * */
    /* the next pointer in the free list... 0&0 means null*/
    /* always first two bytes ... hence why 2 byte size minimum (plus alloc byte)*/
    assign_16(myblock + 2, 0);
    //myblock[2] = 0x0;
    //myblock[3] = 0x0;
    /* footer size*/
    assign_16(myblock + 4998, 5000);
    //myblock[4998] = 0x13;
    //myblock[4999] = 0x88;

    /* Start of first data */
    start = myblock + 2;

    _initialized_ = 1;
  }
}

/* 0 FRONT BIT FOR HEADER AND 0 BACK BIT FOR FOOTER MEANS SIZE 1*/

void assign_16(void *ptr, int size) {
  *((char *) ptr) = (size & 0xFF00) >> 8;
  *(((char *) ptr) + 1) = (size & 0xFF);
}

void split(void *freeblock, void *prevfree, size_t size) {
  char *footer = FOOTER(freeblock);
  int old_size = SIZE(freeblock);
  int oldfreesize = FREESIZE(freeblock);
  char *newfree;

  /* Create the allocated block */

  /* Potentially split */
  /* need 6 bytes for header, footer, 2 bytes min data*/

  /* CASE 1: No previous free pointer and no splitting */
  if (old_size + 6 > size) {
    /* New allocated block */
    assign_16(HEADER(freeblock), size + 1);
    assign_16(FOOTER(freeblock), size + 1);

    /* Split free block */
    assign_16(FOOTER(freeblock) + 2, old_size - size - 4);
    assign_16(footer, old_size - size - 4);

    newfree = FOOTER(freeblock) + 4;
    /* There is a previous pointer in the free list */
    if (prevfree) assign_16(prevfree, FREESIZE(prevfree) + SIZE(freeblock) + 4);
    else {
      start = newfree;
    }

    /* There is a next pointer in the free list */
    if (oldfreesize) {
      assign_16(newfree, oldfreesize - SIZE(freeblock) - 4);
    } else {
      assign_16(newfree, 0);
    }

  } else {
    assign_16(HEADER(freeblock), SIZE(freeblock) + 1);
    assign_16(FOOTER(freeblock), SIZE(freeblock) + 1);

    if (prevfree) {
      if (oldfreesize) {
        /* point prev pointer to one after freeblock */
        assign_16(prevfree, FREESIZE(prevfree) + FREESIZE(freeblock));
      } else {

        assign_16(prevfree, 0);
      }
    } else {
      if (oldfreesize) {
        /* the start of the list is now the following free block */
        start = ((char *) freeblock) + FREESIZE(freeblock);
      } else {
        start = NULL;
      }
    }
  }
}

int main(int argc, char** argv) {

  return 0;
}

void *Malloc(size_t size) {
  set_up(size);
  if (size > 4996) {
    Error(BIG, __LINE__, __FILE__);
    return 0x0;
  }
  if (!start) {
    Error(NO_MEM, __LINE__, __FILE__);
    return 0x0;
  } /*error not enough memory to allocate */

  char *prevfree = 0;
  char *freeblock = start;

  while ((SIZE(freeblock) < size) && FREESIZE(freeblock)) {
    prevfree = freeblock;
    freeblock = NEXT_FREE(freeblock);
  }
  if ((SIZE(freeblock) < size)) {
    Error(SHORT_MEM, __LINE__, __FILE__);
    return 0x0;
  }

  /* Add the offset to the next free block to the prev free block */
  /*Could error if next is odd*/
  split(freeblock, prevfree, size);

  return freeblock;
}

void Free(void *ptr) {
};
