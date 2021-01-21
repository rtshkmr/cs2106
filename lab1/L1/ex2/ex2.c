/*************************************
* Lab 1 Exercise 2
* Name: Fang Junwei, Samuel
* Matric No: A0199163U
* Lab Group: B01
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
    int position, newValue, copies;

    //Fill in code for input and processing
    while (scanf("%i %i %i", &position, &newValue, &copies) > 0)
    {
        //Add to head
        myList = insertAt(myList, position, copies, newValue);
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
    if (position > 0 && head != NULL) {
        head->next = insertAt(head->next, position - 1, copies, newValue);

        return head;
    } else {
        node* newHead = malloc(sizeof(node));
        newHead->data = newValue;
        newHead->next = copies > 1 
            ? insertAt(head, position, copies - 1, newValue) 
            : head;

        return newHead;
    }
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
    while(head) {
        node* nextHead = head->next;

        free(head);
        head = NULL;

        head = nextHead;
    }
}
