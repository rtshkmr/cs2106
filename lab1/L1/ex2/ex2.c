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
		while(scanf("%i %i %i", &position, &newValue, &numberOfCopies) == 3 ) {
				myList = insertAt(myList, position, numberOfCopies, newValue);
		}
		//Output code coded for you
		printf("My List:\n");
		printList(myList);

		destroyList(myList);
		myList = NULL;

		printf("My List After Destroy:\n");
		printList(myList);

		return 0;
}

//Actual Function Implementations
node* insertAt(node* head, int position, int copies, int newValue)
{
		// creates linked list with that specified #copies: 
		node* firstNode = (node*) malloc(sizeof(node));
		firstNode->data = newValue; 
		node* lastNode = firstNode;
		while(copies > 1) {
				node* nextNodePtr = (node*) malloc(sizeof(node));
				nextNodePtr -> data = newValue;
				nextNodePtr -> next = NULL;
				lastNode -> next = nextNodePtr;
				lastNode = nextNodePtr;
				copies--;
		}

		// moves pointers to the insertion point & rejoining pt and joins new linked list:
		if(head == NULL) {
				head = firstNode;
		} else {
				if(position == 0) {
						lastNode -> next = head;
						head = firstNode;
				} else {
						node *insertionAfterNode = head;
						position--; // to add to the left of indicated idx:
						while(position > 0 && insertionAfterNode->next != NULL) { 
								insertionAfterNode = insertionAfterNode->next;
						}
						node *rejoinAtNode = insertionAfterNode->next;
						insertionAfterNode->next = firstNode;
						lastNode->next = rejoinAtNode;
				}
		}
		return head;
}

void printList(node* head)
		//Purpose: Print out the linked list content
		//Assumption: the list is properly null terminated
{
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
