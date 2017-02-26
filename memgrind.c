#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"

void* Malloc(size_t size);
void Free(void* ptr);

void testA(){
	char* ptrs[1000];
	for(int i = 0; i < 1000; i++){
		ptrs[i] = (char*)Malloc(sizeof(char));
	}
	for(int i = 0; i < 1000; i++){
		Free(ptrs[i]);
	}

}

void testB(){
	char* ptrs[1000];
	for(int i = 0; i < 1000; i++){
		ptrs[i] = (char*)Malloc(sizeof(char));
		Free(ptrs[i]);
	}
}

void testC(){
	char* ptrs[1000];
	int m = 0;
	int f = 0;
	for(int i = 0; i < 1000; i++){
		ptrs[i] = NULL;
	}
	while(f < 1000){
		int random = rand() % 2; 
		if(random == 0 && m < 1000){
			ptrs[m] = (char*)Malloc(sizeof(char));
			m++;
		}
		else{
			if(ptrs[f] != NULL){
				Free(ptrs[f]);
				f++;
			}
		}
	}
}

void testD(){
	char* ptrs[1000];
	int m = 0; //count malloc
	int f = 0; //count free
	/*
	 Randomly choose between a randomly-sized malloc() or free()ing a pointer – do this many
 	times (see below)
 	 Keep track of each malloc so that all mallocs do not exceed your total memory capacity
 	*/
 	for(int i = 0; i < 1000; i++){
		ptrs[i] = NULL;
	}
	while(f < 1000){
		int random = rand() % 2; 
		if(random == 0 && m < 1000){
			//Choose a random allocation size between 1 and 64 bytes
			int rSize = rand() % 64 + 1;
			size_t size = (size_t) rSize;
			//- Keep track of each operation so that you eventually malloc() 1000 times
			char* ptr = (char*)Malloc(size);
			if(ptr != NULL){
				ptrs[m] = (char*)Malloc(size);
				m++;
			}
		}
		else{
			//- Keep track of each operation so that you eventually free() all pointers
			if(ptrs[f] != NULL){
				Free(ptrs[f]);
				f++;
			}
		}
	}
}
//test with structs 
//testing memory consistency 
struct testcase{
	int array[100]; 
	char stuff;
};
void testE(){
 	struct testcase* ptrs[10];
 	for(int i = 0; i < 10; i++){
 		ptrs[i] = (struct testcase*)Malloc(sizeof(struct testcase));
 		for(int k = 0; k < 100; k++){
 			ptrs[i]->array[k] = i*10 + k;
 		}
 		ptrs[i]->stuff = i;
 	}
 	for(int i = 0; i < 10; i++){
 		for(int k = 0; k < 100; k++){
 			if(ptrs[i]->array[k] != i*10 + k){
 				printf("this is wrong\n");
 			}	
 		}
 		if(ptrs[i]->stuff != i){
 			printf("this is wrong\n");
 		}	
 		Free(ptrs[i]);
 	}

}

void testF(){
	char* ptrs[1000];
	for(int i = 0; i < 1000; i++){
		ptrs[i] = (char*)Malloc(sizeof(char));
	}
	for(int i = 0; i < 1000; i++){
		char* temp = ptrs[i];
		Free(ptrs[i]);
		ptrs[i] = (char*)Malloc(sizeof(char));
		if(temp != ptrs[i]){
			printf("Is not consistent with first free algorithm.\n");
		}
	}
	for(int i = 0; i < 1000; i++){
		Free(ptrs[i]);
	}

}

double runtime(void (*test)(void)){
	clock_t start = clock();
	for(int i = 0; i < 100; i++){
		test();
	}
	clock_t end = clock();
	return (double)(end - start) / CLOCKS_PER_SEC / 100.0;
}
	
int main(int arg, char* argv[]){

	printf("run testA\n");
	double timeA = runtime(testA);
	printf("Mean time A, %f\n", timeA);
	printf("run testB\n");
	double timeB = runtime(testB);
	printf("Mean time B, %f\n", timeB);
	printf("run testC\n");
	double timeC = runtime(testC);
	printf("Mean time C, %f\n", timeC);
	printf("run testD\n");
	double timeD = runtime(testD);
	printf("Mean time D, %f\n", timeD);
	printf("run testE\n");
	double timeE = runtime(testE);
	printf("Mean time E, %f\n", timeE);
	printf("run testF\n");
	double timeF = runtime(testF);
	printf("Mean time F, %f\n", timeF);



}