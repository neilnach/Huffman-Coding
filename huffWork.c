#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "huffman.h"

//Recursive method that frees a given element and all its children
void freeElement(Node * n){
	if(n){
		freeElement(n->zero);
		freeElement(n->one);
		free(n);	
	}
}

char * getAChar(char c){
	return possibleValues[((int)c) - ASCII_OFFSET];
}


