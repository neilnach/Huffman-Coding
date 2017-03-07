typedef struct Node {
	struct Node * zero;
	struct Node * one;
	char val;
}Node;

Node * head; //head 

int loadBinaryTree(Node * n);

char ** possibleValues; //all ASCII values

int elems;

int ASCII_OFFSET;


