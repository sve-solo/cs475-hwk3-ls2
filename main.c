#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "ls2.h"

//mode1
//get command line args, which means you get the dir to look through
//if there are 2 command line args in argc, then call method sift
//else, don't do anything
//go to method sift
//open the dir
//read the dir
//check if its a dir or regular file with S_ISDIR or S_ISREG
//if it is a file: get the name, and print it out, then get the size, and print it out
//push file name onto stack
//^can even make that its own sep function
//if it's a dir, recursively call sift, passing in the dir
//close dir
//print stack in main

/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 */
int main(int argc, char* argv[]) {

	int indentCounter = 0;
	int indentCounter2 = 0;

	//initialize the stack
    stack_t *stack = initstack();

	//make sure user only enters 2 things for mode1
	if(argc > 3 || argc <= 1){
		printf("Usage: ./ls2 <path> [optional-file]\n");
	}
	else if (argc == 2){
		mode1(argv[1], indentCounter);
	}
	else if(argc == 3){
		mode2(argv[1], argv[2], stack, indentCounter2);
	}
	
	//printf("\n");

	//printf("Stack:\n");
	printstack(stack);

	//free up space
    freestack(stack);
	
	// // stack stores the lines to print out
	// stack_t *s = initstack();

	// push(s, "Hello1");
	// push(s, "Hello2");
	// push(s, "Hello3");

	// // print stack
	// printstack(s);
	
	// // free up stack
	// freestack(s);
	return 0;
}


