#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.h"

void* Malloc(size_t size);
void Free(void* ptr);

/* Allocate 1000 1 bytes and then free them */
void testA(){
	char* ptrs[1000];
	int i;
	for(i = 0; i < 1000; i++){
		ptrs[i] = (char*)Malloc(sizeof(char));
	}
	for(i = 0; i < 1000; i++){
		Free(ptrs[i]);
	}

}

/* Allocate 1 byte and then free this byte.. do it 1000 times */
void testB(){
	char* ptrs[1000];
	int i;
	for(i = 0; i < 1000; i++){
		ptrs[i] = (char*)Malloc(sizeof(char));
		Free(ptrs[i]);
	}
}

/* Randomly allocates or frees 1 byte of memory while making sure
 *  that by the end there are 1000 allocations and 1000 frees */
void testC(){
	char* ptrs[1000];
	int m = 0;
	int f = 0;
	int i;
	for(i = 0; i < 1000; i++){
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

/* Same test as the last but allocation can involve any value from 1-64 */
void testD(){
	int numgen;
	int i;
	int mallocs = 0;
	int frees = 0;
	char *holder[1000];
	for(i = 0; i < 2000; ++i)
	{
		numgen = (rand() % 64) + 1;
		if(mallocs == 1000)
		{
			Free(holder[frees]);
			++frees;
			continue;
		}
		if(mallocs <= frees)
		{
			holder[mallocs] = (char *)Malloc(numgen);
			((long)holder[mallocs]) ? ++mallocs : --i;
			continue;
		}
		else
		{
			if(rand() % 2)
			{
				holder[mallocs] = (char *)Malloc(numgen);
				((long)holder[mallocs]) ? ++mallocs : --i;
				continue;
			}
			else
			{
				Free(holder[frees]);
				++frees;
				continue;
			}
		}
	}
		
}
//test with structs 
//testing memory consistency 
struct testcase{
	int array[10]; 
	char stuff;
};
void testE(){
 	struct testcase* ptrs[50];
	int i;
	int k;
 	for(i = 0; i < 50; i++){
 		ptrs[i] = (struct testcase*)Malloc(sizeof(struct testcase));
 		for(k = 0; k < 10; k++){
 			ptrs[i]->array[k] = i*10 + k;
 		}
 		ptrs[i]->stuff = i;
 	}
 	for(i = 0; i < 50; i++){
 		for(k = 0; k < 10; k++){
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
	char* ptrs[500];
	int i;
	for(i = 0; i < 500; i++){
		ptrs[i] = (char*)Malloc(sizeof(char));
	}
	for(i = 0; i < 500; i++){
		char* temp = ptrs[i];
		Free(ptrs[i]);
		ptrs[i] = (char*)Malloc(sizeof(char));
		if(temp != ptrs[i]){
			printf("Is not consistent with first free algorithm.\n");
		}
	}
	for(i = 0; i < 500; i++){
		Free(ptrs[i]);
	}

}

double runtime(void (*test)(void)){
	clock_t start = clock();
	int i;
	for(i = 0; i < 100; i++){
		test();
	}
	clock_t end = clock();
	return (double)(end - start) / CLOCKS_PER_SEC / 100.0;
}
	
int main(int arg, char* argv[]){

	printf("run testA\n");
	double timeA = runtime(testA);
	printf("run testB\n");
	double timeB = runtime(testB);
	printf("run testC\n");
	double timeC = runtime(testC);
	printf("run testD\n");
	double timeD = runtime(testD);
	printf("run testE\n");
	double timeE = runtime(testE);
	printf("run testF\n");
	double timeF = runtime(testF);


	printf("Mean time A, %f\n", timeA);
	printf("Mean time B, %f\n", timeB);
	printf("Mean time C, %f\n", timeC);
	printf("Mean time D, %f\n", timeD);
	printf("Mean time E, %f\n", timeE);
	printf("Mean time F, %f\n", timeF);
	return 0;
}