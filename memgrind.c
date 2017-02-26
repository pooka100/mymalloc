#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"


extern static char* myblock;

void testA(){
	char* ptrs[1000];
	for(int i = 0; i < 1000; i++){
		ptrs[i] = *Malloc(sizeof(char));
	}
	for(int i = 0; i < 1000; i++){
		Free(ptrs[i]);
	}

}

void testB(){
	char* ptrs[1000];
	for(int i = 0; i < 1000; i++){
		ptrs[i] = *Malloc(sizeof(char));
		Free(ptrs[i]);
	}
}

void testC(){
	char* ptrs[1000];
	int m = 0;
	int f = 0;
	while(f < 1000){
		int random = rand() % 2; 
		if(random == 0){
			ptrs[m] = *Malloc(sizeof(char));
			m++;
		}
		else{
			if(ptrs[f] != null){
				Free(ptrs[f]);
				f++;
			}
		}
	}
}

void testD(){
	char* ptrs[1000];
	int m = 0;
	int f = 0;
	int max = 0
	/*
	 Randomly choose between a randomly-sized malloc() or free()ing a pointer – do this many
 	times (see below)
 	 Keep track of each malloc so that all mallocs do not exceed your total memory capacity
 	*/
	while(f < 1000 && max < 5000){
		int random = rand() % 2; 
		if(random == 0 && m < 1000){
			//Choose a random allocation size between 1 and 64 bytes
			int rSize = rand() % 64 + 1;
			size_t size = (size_t) rSize;
			//- Keep track of each operation so that you eventually malloc() 1000 times
			ptrs[m] = *Malloc(size);
			max += rSize;
			m++;
		}
		else{
			//- Keep track of each operation so that you eventually free() all pointers
			if(ptrs[f] != null){
				Free(ptrs[f]);
				f++;
			}
		}
	}
	
	
}
int main(int arg, char* argv[]){


}