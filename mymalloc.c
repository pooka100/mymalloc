#include <sys/time.h>

#include "mymalloc.h"



void getfrees()
{
	char *thing = start;
	if(start == NULL) return;
	while(FREESIZE(thing) != 0)
	{
		printf("%ld -> %ld ", thing, FOOTER(thing));
		thing = NEXT_FREE(thing);
	}

		printf("%ld -> %ld ", thing, FOOTER(thing));
}

void Error(enum _errors err, int line, char *file) {
  printf("Error at line: %d in %s:\n", line, file);

  switch (err) {
    case NO_MEM: printf("No memory available for allocation\n"); break;
    case SHORT_MEM: printf("Not enough memory available for allocation\n"); break;
    case BIG: printf("Size requested larger than heap size\n"); break;
		case NO_ALLOC: printf("No memory has been allocated");
		case OOR: printf("Requested allocated memory is out of range of the heap\n"); break;
		case INVALID: printf("Requested allocated memory is either already free or not the head of an allocated block\n"); break;
  }
}

/* declared only once so just inline */
inline void set_up(size_t size) {
  /* Make size a multiple of 2 */

  /* equals 5000 together for size, no 1 bit at end because free*/
  if (!_initialized_) {
    assign_16(myblock, 4996);
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
    assign_16(myblock + 4998, 4996);
    //myblock[4998] = 0x13;
    //myblock[4999] = 0x88;

    /* Start of first data */
    start = myblock + 2;
		end = start;

    _initialized_ = 1;
  }
}

/* 0 FRONT BIT FOR HEADER AND 0 BACK BIT FOR FOOTER MEANS SIZE 1*/

void assign_16(void *ptr, int size) {
  *((char *) ptr) = (size & 0xFF00) >> 8;
  *(((char *) ptr) + 1) = (size & 0xFF);
}

void assign_0(void *ptr)
{
	/* header already comes with 0'd out 1st bit */
	int s = SIZE(ptr);
	assign_16(HEADER(ptr), s);
	assign_16(FOOTER(ptr), s);
}

void *merge_left(void *ptr)
{
	int total_size = SIZE(ptr) + SIZE(PREV(ptr)) + 4;

	ptr = PREV(ptr);
	assign_16(HEADER(ptr), total_size);
	assign_16(FOOTER(ptr), total_size);
	return ptr;
}

void *merge_right(void *ptr)
{
	int total_size = SIZE(ptr) + SIZE(NEXT(ptr)) + 4;
	/* It will obviously be the next item in the free list */

	if (!FREESIZE(NEXT(ptr))) assign_16(ptr, 0);
	else assign_16(ptr, FREESIZE(NEXT(ptr)) + 4 + SIZE(ptr));
	

	assign_16(HEADER(ptr), total_size);

	//WTFFFFF 
	assign_16(FOOTER(ptr), total_size);
	return ptr;	
}

char *get_prevfree(void *ptr)
{
	/* returns the previous free pointer directly before the about to be deallocated block... returns NULL if no such pointer exists*/
	void *tmp = start;
	void *tmp2 = 0;
	/* if there is no freelist then there is no next*/
	if(!tmp) return 0;	
	while((tmp <= ptr) && (tmp != 0))
	{
		/* if this is the end of the freelist then this is the last item */
		if(!FREESIZE(tmp)) return (char *)tmp;
		tmp2 = tmp;
		tmp = NEXT_FREE(tmp);
	}
	if(tmp )

	return (char *)tmp2;
}

char *get_nextfree(void *ptr)
{
	void *tmp = start;
	/* if there is no freelist then there is no next*/
	if(!tmp) return 0;
	while((tmp < ptr) && (tmp != 0))
	{
		/* if there is no next to this lesser pointer.. then there is no next */
		if(!FREESIZE(tmp)) return 0;
		tmp = NEXT_FREE(tmp);
	}
	printf("NEXT FREE: %ld\n", tmp);
	return (char *)tmp;
}

void *coalesce(void *ptr)
{
	/* Checks if front or back of heap.. takes advantage of short circuit in if statement */
	int beginning = 0;
	int end = 0;

	if(ptr == myblock + 2) beginning = 1;
	if(FOOTER(ptr) == myblock + 4998) end = 1;

	char *prevptr = (char *)get_prevfree(ptr);
	char *nextptr = (char *)get_nextfree(ptr);
	/* set ALLOC to 0 */
	assign_0(ptr);

	if(beginning && end){ 
		assign_16(ptr, 0);	
		start = ptr;
		return ptr; }
	if(beginning && !end)
	{
		printf("GOTCHA\n");
		/* this will be the start pointer */
		if(ALLOC(NEXT(ptr)) == 0)
		{
			printf("DAMN SON\n");
			ptr = merge_right(ptr);	
			
			start = ptr;
			return ptr;
		}
		else
		{
			/* if there is another pointer in front */
			if(nextptr){ assign_16(ptr, ((int)((long)nextptr - (long)ptr)));
			printf("THE SIZE WAS: %d\n", (int)((long)nextptr - (long)ptr));}
			else assign_16(ptr, 0);
			start = ptr;
			return ptr;
		}
	}

	if(!beginning && end)
	{
		printf("TO THE BRIG\n");
		assign_16(ptr, 0);
		if(!ALLOC(PREV(ptr)))
		{
			ptr = merge_left(ptr);
			return ptr;
		}
		else
		{
			if(prevptr)
			{
				//if((int)(FREESIZE(prevptr))) assign_16(ptr, FREESIZE(prevptr) - (int)((long)ptr - (long)prevptr));
				assign_16(prevptr, (int)((long)ptr - (long)prevptr));
			}
			else
			{
				start = ptr;
			}
			return ptr;
		}
	}

	if(!ALLOC(PREV(ptr)) && ALLOC(NEXT(ptr)))
	{
		//THIS FUCKER
		printf("TO THE BRIG2\n");
		assign_16(ptr, 0);
		ptr = merge_left(ptr);

		return ptr;
	}
	else if(ALLOC(PREV(ptr)) && !ALLOC(NEXT(ptr)))
	{
		printf("TO THE BRIG3\n");
		//if(prevptr) assign_16(prevptr, FREESIZE(prevptr) - 4 - SIZE(ptr));
		if(prevptr) assign_16(prevptr, (int)((long)ptr - (long)prevptr));
		else start = ptr;
		ptr = merge_right(ptr);
		return ptr;
	}
	else if(!ALLOC(PREV(ptr)) && !ALLOC(NEXT(ptr)))
	{
		printf("TO THE BRIG4\n");
		if(FREESIZE(NEXT(ptr)))
		{
		  ptr = merge_left(ptr);
			ptr = merge_right(ptr);
		}
		else
		{
			assign_16(ptr, 0);

	
			ptr = merge_right(ptr);
		  ptr = merge_left(ptr);
		}
		return ptr;
	}
	else
	{
		printf("TO THE BRIG5\n");
		if(prevptr)
		{
			assign_16(prevptr, (int)((long)ptr - (long)prevptr));
		}
		else start = ptr;
		if(nextptr)
		{
			assign_16(ptr, (int)((long)nextptr - (long)ptr));
		}
		else assign_16(ptr, 0);
		return ptr;
	}
	
}


void split(void *freeblock, void *prevfree, size_t size) {
  char *footer = FOOTER(freeblock);
  int old_size = SIZE(freeblock);
	char *nextfree;
	if(FREESIZE(freeblock))
	{
		nextfree = NEXT_FREE(freeblock);
	} else
	{
		nextfree = 0;
	}
  char *newfree;

  /* Create the allocated block */

  /* Potentially split */
  /* need 6 bytes for header, footer, 2 bytes min data*/
  if (old_size > size + 4) {
    /* New allocated block */
    assign_16(HEADER(freeblock), size + 1);
    assign_16(FOOTER(freeblock), size + 1);
    /* Split free block */
    assign_16(FOOTER(freeblock) + 2, old_size - size - 4);
    assign_16(footer, old_size - size - 4);

    newfree = FOOTER(freeblock) + 4;
    /* There is a previous pointer in the free list */
    if (prevfree)
		{
			assign_16(prevfree, (int)(((long)newfree) - ((long)prevfree)));
		}
    else {
      start = newfree;
    }
		
		if(nextfree)
		{
			assign_16(newfree, (int)(((long)nextfree) - ((long)newfree)));
		}
		else
		{
			assign_16(newfree, 0);
		}

    /* There is a next pointer in the free list */

  } else {
    assign_16(HEADER(freeblock), SIZE(freeblock) + 1);
    assign_16(FOOTER(freeblock), SIZE(freeblock) + 1);

    if (prevfree) {
      if (nextfree) {
        /* point prev pointer to one after freeblock */
        assign_16(prevfree, (int)(((long)nextfree) - ((long)prevfree)));
      } else {

        assign_16(prevfree, 0);
      }
    } else {
      if (nextfree) {
        /* the start of the list is now the following free block */
        start = nextfree;
      } else {
        start = NULL;
      }
    }
  }

}

int main(int argc, char** argv) {

	char **holder; //= (char **)malloc(sizeof(char *) * 1000);
/*
	int i;
	for(i = 0; i < 1000; ++i)
	{
		holder[i] = (char *)Malloc(1);
	}
	for(i = 0; i < 1000; ++i)
	{
		Free(holder[i]);
	}
	free(holder);
	printf("STAGE 1 COMPLETED\n");
	
	char *bolder;
	for(i = 0; i < 1000; ++i)
	{
		bolder = (char *)Malloc(1);
		Free(bolder);
	}
	printf("STAGE 2 COMPLETED\n");	
	*/
	int i;
	int mallocs = 0;
	int frees = 0;
	
	struct timeval time;
	
 /*for(i = 0; i < 2000; ++i)
	{
			
	}
	*/
	gettimeofday(&time, 0);
	srand((unsigned)time.tv_usec);
	holder = (char **)malloc(sizeof(char *) * 1000);
	printf("%ld\n", myblock + 2);	
	for(i = 0; i < 2000; ++i)
	{
		getfrees();	
		printf("%ld\n", start);
		if(mallocs == 1000)
		{
			Free(holder[frees]);
			printf("did a free\n");
			++frees;
			continue;
		}
		if(mallocs <= frees)
		{
			holder[mallocs] = (char *)Malloc(1);
			((long)holder[mallocs]) ? ++mallocs : --i;
			printf("did an allocate\n");
			continue;
		}
		else
		{
			if(rand() % 2)
			{
				holder[mallocs] = (char *)Malloc(1);
				((long)holder[mallocs]) ? ++mallocs : --i;
			printf("did an allocate\n");
				continue;
			}
			else
			{
				Free(holder[frees]);
				printf("did a free\n");
				++frees;
				continue;
			}
		}
	}


	
	free(holder);
  return 0;
}

void *Malloc(size_t size) {
  set_up(size);
  size = (size & 0x1) ? size + 1 : size;
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
	if(!_initialized_) { Error(NO_ALLOC, __LINE__, __FILE__); return;}
	/* safe way */

	if(((long)ptr >= (long)(myblock + 4996)) || ((long)ptr <= (long)(myblock + 1))) {Error(OOR, __LINE__, __FILE__); return;}
	
	if(ALLOC(ptr) == 0) { Error(INVALID, __LINE__, __FILE__); return;}
	printf("SIZE PTR: %d\n", SIZE(ptr));
	coalesce(ptr);
};
