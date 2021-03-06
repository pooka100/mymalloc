CC=gcc
CFLAGS=-c -Wall



all: memgrind

memgrind: mymalloc.o memgrind.o
	$(CC) mymalloc.o memgrind.o -o memgrind

mymalloc.o: mymalloc.c
	$(CC) $(CFLAGS) mymalloc.c

memgrind.o: memgrind.c
	$(CC) $(CFLAGS) memgrind.c

clean:
	rm memgrind mymalloc.o memgrind.o 
