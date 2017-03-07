C = gcc
CFLAGS= -Wall -Werror -g

btree: huffman.c huffWork.c 
	$(CC) $(CFLAGS) -o huffman huffman.c huffWork.c

	clean:
		rm -f huffman
