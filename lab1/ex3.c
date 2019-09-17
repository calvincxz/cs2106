#include <stdio.h>
#include <stdlib.h>
// stdlib includes malloc() and free()

// define instructions
#define PREVIOUS -1
#define NEXT 1
#define DELETE 0
#define INSERTSUBNODE 2
#define COLLAPSE 3 // New instruction compared to ex2.

// Node declarations

typedef struct SUBNODE
{
	int data;
	struct SUBNODE* nextSubNode;
} subNode;

typedef struct NODE
{
	int data;
	struct NODE* previousNode;
	struct NODE* nextNode;
	subNode* subNodeHead;
} node;

// Function prototypes 

void insertNodeNext(int,node*); 
void insertNodePrevious(int,node*);
void insertSubNode(int,node*);
void deleteNode(int,node*);
void deleteList(node*);
void deleteAllSubNodes(node*);
void collapseSubNodes(int, node*); //TODO 

void printList(node*);
void printSubNodes(node*);



// Start of main
int main()
{
	int position;
	int instruction;
	int subPosition;
	int value;
	
	// Declaration of the origin Node
	node* originNode = (node*)malloc(sizeof(node));
	originNode->previousNode = originNode;
	originNode->nextNode = originNode;
	originNode->data = 0;
	originNode->subNodeHead = NULL;

#define PREVIOUS -1
#define NEXT 1
#define DELETE 0
#define INSERTSUBNODE 2
#define COLLAPSE 3 // New instruction compared to ex2.
	void (*functions[7])();
	functions[0] = &insertNodePrevious;
	functions[1] = &deleteNode;
	functions[2] = &insertNodeNext;
	functions[3] = &insertSubNode;
	functions[4] = &collapseSubNodes;
	functions[5] = &printList;
	functions[6] = &deleteList;


	
	while(scanf("%i%i",&instruction,&position) == 2)
		//scanf() returns an int that corresponds to the number of values scanned. 
	{
		(functions[instruction+1])(position,originNode);
		
	}
	(functions[5])(originNode);
	(functions[6])(originNode);
	
	printf("Circular List after delete\n");
	(functions[5])(originNode);
	
	free(originNode);
	
	return 0;
}

//Function Implementations 

void collapseSubNodes(int position,node* targetNode)
{
	//finding targetnode
	node *iterator = targetNode;
	int count = 0;
	while (count < position) {
        iterator = iterator -> nextNode;
        count++;
    }
    //adding values of subnodes
	int sum = iterator->data;
	subNode* temp = iterator->subNodeHead;
	while(temp!=NULL) 
	{   
		sum += temp->data;
		temp = temp->nextSubNode;
	}
	iterator->data = sum;
	deleteAllSubNodes(iterator);
}

void insertSubNode(int position,node* originNode)
{
	int subPosition;
	int value;
	scanf("%i%i",&subPosition,&value);
	node *iterator = originNode;
	subNode* newSubNode = (subNode*)malloc(sizeof(subNode));
	newSubNode->data = value;
	int count = 0;
    while (count < position) {
        iterator = iterator -> nextNode;
        count++;
    }
    subNode *pointer = iterator -> subNodeHead;
    if (pointer == NULL) {
    	iterator -> subNodeHead = newSubNode;
    	newSubNode-> nextSubNode = NULL;
    } else if (pointer->nextSubNode == NULL && subPosition == 0) {
    	newSubNode-> nextSubNode = pointer;
    	iterator -> subNodeHead = newSubNode;
    } else if (pointer->nextSubNode == NULL && subPosition == 1) {
    	newSubNode-> nextSubNode = NULL;
    	iterator -> subNodeHead -> nextSubNode= newSubNode;
    } else { //at least 2 subnode
    	int count2 = 0;
	    while (pointer->nextSubNode != NULL && count2 < subPosition - 1) {
	    	pointer = pointer -> nextSubNode;
	    	count2++;
	    }
	    newSubNode-> nextSubNode = pointer->nextSubNode;
    	pointer -> nextSubNode = newSubNode;
    }
    
}

void deleteAllSubNodes (node* targetNode)
{
	subNode* temp = targetNode->subNodeHead;
	while(temp!=NULL) {   
		targetNode->subNodeHead = temp->nextSubNode;
		free(temp);
	    temp = targetNode->subNodeHead;
	}
}

void insertNodePrevious(int position, node* originNode)
{
	int value;
	scanf("%i",&value);
	node *iterator = originNode;
	node* newNode = (node*)malloc(sizeof(node));
	int count = 0;
    while (count < position) {
        iterator = iterator -> nextNode;
        count++;
    }
    node *prev = iterator->previousNode;
    iterator -> previousNode = newNode;
    newNode->data = value;
	newNode->previousNode = prev;
	prev->nextNode = newNode;
	newNode->nextNode = iterator;
	newNode->subNodeHead = NULL;

}

void insertNodeNext(int position, node* originNode)
{
	int value;
	scanf("%i",&value);
	node *iterator = originNode;
	node* newNode = (node*)malloc(sizeof(node));
	int count = 0;
    while (count < position) 
    {
        iterator = iterator -> nextNode;
        count++;
    }
    node *next = iterator->nextNode;
	iterator -> nextNode = newNode;
    newNode->data = value;
	newNode->previousNode = iterator;
	next -> previousNode = newNode;
	newNode->nextNode = next;
	newNode->subNodeHead = NULL;
   
	
}

void deleteNode (int position,node* originNode)
{
	int count = 0;
	node* prev;
	node* next;
	node* iterator = originNode;
	while(count < position)
	{
		iterator = iterator->nextNode;
		count++;
	}
	prev = iterator->previousNode;
	prev -> nextNode = iterator -> nextNode;
	next = iterator -> nextNode;
	next -> previousNode = iterator -> previousNode;
	//deleteAllSubNodes (iterator);
	free(iterator);
	iterator = NULL;
	
}

void deleteList(node* originNode)
{
	node* temp1 = originNode->nextNode;
	while(temp1 != originNode) 
	{   
		deleteAllSubNodes (temp1);
	    deleteNode(1, originNode);
	    temp1 = originNode->nextNode;
	}
	deleteAllSubNodes(originNode);
}

//Print list has been implemented for you. 
void printList(node* originNode)
{
	int count = 0;
	printf("Printing clockwise:\n");
	node* iterator = originNode->nextNode;
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);
	printf("\n   |\n   v\n");
	while(iterator != originNode)
	{
		count++;
		printf("[Pos %i:%i]",count,iterator->data);
		// printing subNodes
		printSubNodes(iterator);
		printf("\n   |\n   v\n");
		iterator = iterator->nextNode;
	}
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);		
	
	printf("\nPrinting counter-clockwise:\n");
	iterator = originNode->previousNode;
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);
	printf("\n   |\n   v\n");
	while(iterator!= originNode)
	{
		printf("[Pos %i:%i]",count,iterator->data);
		// printing subNodes
		printSubNodes(iterator);
		printf("\n   |\n   v\n");
		iterator = iterator->previousNode;
		count--;
	}
	printf("[Pos %i:%i]",0,originNode->data);	
	// printing subNodes
	printSubNodes(originNode);
	printf("\n");

	return;
}

void printSubNodes(node* mainNode)
{
	int count = 0;
	subNode* iterator = mainNode->subNodeHead;
	while(iterator != NULL)
	{
		printf("->[subNode pos %i:%i]",count,iterator->data);
		iterator = iterator->nextSubNode;
		count++;
	}
}
