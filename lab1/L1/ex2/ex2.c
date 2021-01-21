/*************************************
* Lab 1 Exercise 1
* Name: Ritesh Kumar
* Matric No: A0201829H
* Lab Group: 12
*************************************/

#include <stdio.h>
#include <stdlib.h> //for malloc() and free()

//Declaration of a Linked List Node

typedef struct NODE{
    int data;
    struct NODE* next;
} node;

//Function Prototypes
node* insertAt(node*, int, int, int);

void printList(node*);
void destroyList(node*);



int main()
{
    node* myList = NULL;    //Empty List
    int position, newValue, numberOfCopies; // input values

	//Fill in code for input and processing
	while(scanf("%i %i %i", &position, &newValue, &numberOfCopies) == 1 ) {
		printf("testing");
		myList = insertAt(myList, position, numberOfCopies, newValue);
	}
    //Output code coded for you
    printf("My List:\n");
    printList(myList);
   return 0; 
    destroyList(myList);
    myList = NULL;
    
    printf("My List After Destroy:\n");
    printList(myList);
 
    return 0;
}

//Actual Function Implementations
node* insertAt(node* head, int position, int copies, int newValue)
{
	// creates linked list with that many copies: 
    node* firstPtr = (node*) malloc(sizeof(node));
	node* currentNode = firstPtr;
	while(copies > 0) {
		node* nextNodePtr = (node*) malloc(sizeof(node));
		nextNodePtr->data = newValue;
		nextNodePtr->next = NULL;
		currentNode->data = newValue; 
		currentNode->next = nextNodePtr;
		currentNode = nextNodePtr;
		copies--;
	}

	if(head == NULL) { 
		return firstPtr;
	} else {
	    // finds insertion point and joining point w.r.t original linked list:
		node* insertPosition = head;
		while(position > 0) { // shift pointer to idx x - 1
			if(head->next == NULL) break;
			insertPosition = head->next;
			position--;
		}
		node* joinPosition = insertPosition->next;
		// set pointers appropriately:
		insertPosition->next = firstPtr;
		currentNode->next = joinPosition;	
		//Fill in your code here
		return head;    //change this!
	}
}
 
void printList(node* head)
//Purpose: Print out the linked list content
//Assumption: the list is properly null terminated
{
	printf("printing list");
    //This function has been implemented for you
    node* ptr = head;

    while (ptr != NULL)  {    //or you can write while(ptr)
        printf("%i ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

void destroyList(node* head)
{
    //Fill in your code here
    //You can use the same implementation as in exercise 1
	if(head->next == NULL) {
		free(head);
	} else {
		node* sublist = head->next;
		free(head);
		destroyList(sublist);
	}
}
