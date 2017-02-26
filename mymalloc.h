#include <stdio.h>
#include <stdlib.h>
#include <time.h>
enum _errors 
{
	NO_MEM, SHORT_MEM, BIG, NO_ALLOC, INVALID, OOR
};
/* Heap space */
static char myblock[5000];
/* Start of singly linked list for free partitions */
static char *start;

static int _initialized_ = 0;
/* Largest free block */
/* Macros: expects start of data(allocated)/list(free) */
/* last bit defines allocation so even number amounts only */
#define    HEADER(X) (((char *)X) - 2)
#define      SIZE(X) ((int)(*(HEADER(X)) << 8) + ((*(HEADER(X+1)) & 0xFE)))
#define  PREVSIZE(X) SIZE(HEADER(X))
#define    FOOTER(X) (((char *)X) + SIZE(X))
#define      NEXT(X) (FOOTER(X) + 4)
#define      PREV(X) (HEADER(X) - PREVSIZE(X) - 2)
#define     ALLOC(X) (int)(*(HEADER(X+1)) & 0x1)
#define  FREESIZE(X) SIZE(X+2)
#define NEXT_FREE(X) (((char *)X) + FREESIZE(X))

inline void  set_up(size_t size);
void         assign_16(void *ptr, int size);
void          assign_0(void *ptr);
void         *merge_left(void *ptr);
void        *merge_right(void *ptr);
char        *get_prevfree(void *ptr);
char        *get_nextfree(void *ptr);
void         split(void *freeblock, void* prevfree, size_t size);
void         Error(enum _errors err, int line, char *file);
void         *coalesce(void *ptr);

void *Malloc(size_t size);
void  Free(void *ptr);



