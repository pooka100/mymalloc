/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mymalloc.h
 * Author: jamesdehart
 *
 * Created on February 19, 2017, 10:43 PM
 */

#ifndef MYMALLOC_H
#define MYMALLOC_H

/* Heap space */
static char myblock[5000];
/* Start of singly linked list for free partitions */
static char *start;

/* Macros: expects start of data/list */
/* Only first 13 bits of 16 byte header are used for size */
/* Next 2 bits are reserved and the last bit booleans allocation (0 = not allocated/ 1 = allocated) */
#define    HEADER(X) ((char *)X) - 2
#define      SIZE(X) (*(HEADER(X)) << 8) + ((*(HEADER(X+1)) & 0x254) 
#define    FOOTER(X) ((char *)X) + SIZE(X)
#define      NEXT(X) FOOTER(X) + 4
#define      PREV(X) HEADER(X) - SIZE(HEADER(X)) - 2
#define     ALLOC(X) *(HEADER(X+1)) & 0x1
#define NEXT_FREE(X) ((char *)X) + SIZE(HEADER(X)+4) 



void *Malloc(size_t size);
void  Free(void *ptr);


/*Free functions*/
#endif /* MYMALLOC_H */

