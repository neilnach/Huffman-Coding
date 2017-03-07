#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "huffman.h"

void deleteTree(){
	//TODO: Write a function that deletes(frees) the data structure
	//used during the decompression algorithm
	if (!head) {
		return;
	}
	freeElement(head -> zero);
	freeElement(head -> one);
	free(head);
}

void deleteList(){
	//TODO: Write a function that deletes(frees) the data
	//structure you were using during your compression algorithm
	if(!elems){
		return;
	}
	int i = 0;
	while (i != elems) {
		if(possibleValues[i]){
			free(possibleValues[i]);
		}
		i++;
	}
	free(possibleValues);
}

char *trim(char *str)
{
	char *end;

	while(isspace((unsigned char)*str)) str++;

	if(*str == 0)  
		return str;

	end = str + strlen(str) - 1;
	while(end > str && isspace((unsigned char)*end)) end--;

	*(end+1) = 0;

	return str;
}

void loadCode(char * filename, int comp){
	if(comp){
		possibleValues = malloc(96*sizeof(char *));
		ASCII_OFFSET = 32;
		elems = 96;
		return 0;
	}else{
		head = malloc(sizeof(Node));
		head -> zero = NULL;
		head -> one = NULL;
		return head;
	}

	FILE *f = fopen(filename, "r");
	char buff[256];
	while(fgets(buff, 256, f) != NULL){
		char * token = strtok(buff, "=");
		char c = *token;
		token = trim(strtok(NULL, "="));
		if(comp){
			add(c, token);
			if(c <32 || c > 127){
				printf("Error: ASCII Value out of printable range\n");
				return -1;
			}
			addrs[((int)c) - ASCII_OFFSET] = strdup(addr);
			return 0;
		}else{
			addr(c, token);
		}
	}
	fclose(f);
}

void huffmanCompress(char * fileName, char * output) {
	if (!possibleValues) {
		printf("ERROR: Loaded improperly");
	}
	FILE * file = fopen(fileName, "r");
	FILE * out = fopen(output, "wb");
	int numBits = 0;
	unsigned char bitsString = 0;
	char * s;
	char c = (char)fgetc(file);
	while (c != EOF) {
		s = getAChar(c);
		int i = 0;
		while(i < strlen(s)){
			if(s[i] == '0'){
				bitsString<<=1;
				numBits++;
			}
			else if(s[i] == '1'){
				bitsString<<=1;
				bitsString += 1;
				numBits++;
			}
			else{
				printf("ERRORs\n");
			}
			if(numBits == 8){
				fwrite(&bitsString, sizeof(unsigned char), 1, out);
				numBits = 0;
				bitsString = 0;
			}
			i++;
		}
		c = (char)fgetc(file);
	}
	for (numBits; numBits < 8; numBits++) {
		bitsString<<=1;
	}
	fwrite(&bitsString, sizeof(unsigned char), 1, out);
	numBits = 0;
	bitsString = 0;
	fclose(file);
	fclose(out);
}
/*	Parse the command inputs:
 *
 *	argv[0] is always program name
 *	-c or -d:			will determine whether to compress or decompress: -c or -d 
 *	<PATHTOSOURCE>:		will determine what file to compress(or decompress)(Error if it is a folder)
 *	--code:				is a flag to say that the next argument is the file with the huffman codes
 *	<PATHTOCODE>:		is the path to the huffman codes file
 *	-p:(optional) 		to specify a destination file.
 *	<PATHTODEST>:		(if -p is there) specifies the filename to compress(decompress) to.
 *		Default destination path(no -p flag) is out.huf and out.dec for compress and decompress, respectively
 *		if invalid path, error and exit.
 *
 *  NOTE:  ARGUMENTS CAN BE IN ANY ORDER, AS LONG AS EACH PATH
 IS PRECEDED BY ITS RESPECTIVE FLAG
 *    This means that it could be:
 *		./huffman --code <PATHTOCODE> -c <PATHTOSOURCE>    or
 *		./huffman -d <PATHTOSOURCE> -p <PATHTODEST> --code <PATHTOCODE>    or
 *		./huffman -p <PATHTODEST> -d <PATHTOSOURCE> --code <PATHTOCODE>
 * 
 *		etc..............
 *
 *	valid syntax shorthand:
 *	./huffman <-c|-d> <PATHTOSOURCE> --code <PATHTOCODE> [-p <PATHTODEST>]
 */



int main(int argc, char const *argv[]) {
	if((argc-1) % 2 != 0 || argc == 1){
		printf("Usage: <-c|-d> <PATHTOSOURCE> --code <PATHTOCODE> [-p <PATHTODEST>]\n");
		return -1;
	}
	char * filedecsrc = NULL;
	char * filecompsrc = NULL;
	char * codefile = NULL;
	char * fileout = NULL;

	int i;
	for(i = 0; i < argc; i++){
		if(!strcmp(argv[i], "--code")){
			codefile = strdup(argv[i+1]);
			break;
		}
	}
	if(!codefile){
		printf("Error: No huffman code file parameter\n");
		return -1;
	}

	for(i = 0; i < argc; i++){
		if(!strcmp(argv[i], "-p")){
			fileout = strdup(argv[i+1]);
			break;
		}
	}

	for(i = 0; i < argc; i++){
		if(!strcmp(argv[i], "-c")){
			filecompsrc = strdup(argv[i+1]);
			if(!fileout){
				fileout = "out.huf";
			}
			break;
		}else if(!strcmp(argv[i], "-d")){
			filedecsrc = strdup(argv[i+1]);
			if(!fileout){
				fileout = "out.dec";
			}
			break;
		}
	}
	if(filecompsrc){
		loadCode(codefile, 1);
		compress(filecompsrc, fileout);
		deleteList();
		free(codefile);
		free(filecompsrc);
		free(fileout);
	}
}
