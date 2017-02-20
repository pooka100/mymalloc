/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: jamesdehart
 *
 * Created on February 19, 2017, 10:43 PM
 */

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
	int x = 5;
	printf("%d", x << 1);
	return 0;
}

void *Malloc(size_t size)
{
	/* Make size a multiple of 16 */
	
}


void Free(void *ptr);