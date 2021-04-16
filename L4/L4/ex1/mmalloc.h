#ifndef __MMALLOC_H_	//standard protection against multiple includes
#define __MMALLOC_H_

#include <unistd.h>	//for brk, sbrk

/**********************************************************
 * There is no need to modify this file for this exercise
 *********************************************************/

#define FREE 0
#define OCCUPIED 1

/**********************************************************
 * Structure to describe a single partition.
 * Can be chained via "nextPart" to form a singly linked list
 *********************************************************/
typedef struct PARTINFO {
    unsigned int offset;
	int	size;
	struct PARTINFO *nextPart;
	int status;
} partInfo;

/**********************************************************
 * Structure to maintain the heap book keeping information
 *********************************************************/
typedef struct {
	int totalSize;			//Total "heap" size in bytes
    void* base;				//Address of the contiguous piece of "heap" memory
	partInfo* pListHead;	//Linked list head pointer to the partition information
} heapMetaInfo;

int setupHeap(int);
void printHeapMetaInfo();
void printHeapStatistic();
void printHeap();

void* mymalloc(int);
void myfree(void*);

#endif
