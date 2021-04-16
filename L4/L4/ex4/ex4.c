/*************************************
* Lab 4 Exercise 3
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmalloc.h"

/**********************************************************
 * This is a "global" structure storing heap information
 * The "static" modifier restrict the structure to be
 *  visible only to functions in this file
 *********************************************************/
static heapMetaInfo hmi;
static int minLvl;
static int maxLvl;

/**********************************************************
 * Quality of life helper functions / macros
 *********************************************************/
#define powOf2(E) (1 << E)

unsigned int log2Ceiling( unsigned int N )
/**********************************************************
 * Find the smallest S, such that 2^S >= N
 * S is returned
 *********************************************************/
{
    unsigned int s = 0, pOf2 = 1;

    while( pOf2 < N){
        pOf2 <<= 1;
        s++;
    }

    return s;
}


unsigned int log2Floor( unsigned int N )
/**********************************************************
 * Find the largest S, such that 2^S <= N
 * S is returned
 *********************************************************/
{
    unsigned int s = 0, pOf2 = 1;

    while( pOf2 <= N){
        pOf2 <<= 1;
        s++;
    }

    return s-1;
}

unsigned int buddyOf( unsigned int addr, unsigned int lvl )
/**********************************************************
 * Return the buddy address of address (addr) at level (lvl)
 *********************************************************/
{
    unsigned int mask = 0xFFFFFFFF << lvl;
    unsigned int buddyBit = 0x0001 << lvl;

    return (addr & mask) ^ buddyBit;
}

partInfo* buildPartitionInfo(unsigned int offset)
/**********************************************************
 * Allocate a new partInfo structure and initialize the fields
 *********************************************************/
{
    partInfo *piPtr;

    piPtr = (partInfo*) malloc(sizeof(partInfo));

    piPtr->offset = offset;
	piPtr->nextPart = NULL;

    //Buddy system's partition size is implicit
	//piPtr->size = size;

    //All available partition in buddy system is implicitly free
	//piPtr->status = FREE;

    return piPtr;
}

void printPartitionList(partInfo* piPtr)
/**********************************************************
 * Print a partition linked list
 *********************************************************/
{
	partInfo* current;
    int count = 1;
	
	for ( current = piPtr; current != NULL; 
		current = current->nextPart){
        if (count % 8 == 0){
            printf("\t");
        }
		printf("[+%5d] ", current->offset);
        count++;
        if (count % 8 == 0){
            printf("\n");
        }
	}
    printf("\n");
}

void printHeapMetaInfo()
/**********************************************************
 * Print Heap Internal Bookkeeping Information
 *********************************************************/
{
    int i;

	printf("\nHeap Meta Info:\n");
	printf("===============\n");
	printf("Total Size = %d bytes\n", hmi.totalSize);
	printf("Start Address = %p\n", hmi.base);

    for (i = hmi.maxIdx; i >=0; i--){
        printf("A[%d]: ", i);
        printPartitionList(hmi.A[i] );
    }

}

void printHeap()
/**********************************************************
 * Print the content of the entire Heap 
 *********************************************************/
{
    //Included as last debugging mechanism.
    //Print the entire heap regions as integer values.

    int* array;
    int size, i;
    
    size = hmi.totalSize / sizeof(int);
    array = (int*)hmi.base;

    for ( i = 0; i < size; i++){
        if (i % 4 == 0){
            printf("[+%5d] |", i);
        }
        printf("%8d",array[i]);
        if ((i+1) % 4 == 0){
            printf("\n");
        }
    }
}

void printHeapStatistic()
/**********************************************************
 * Print Heap Usage Statistics
 *********************************************************/
{
    //TODO: Task 4. Calculate and report the various statistics
    
    int numParts = 0;
    int freeSize = 0;

    for (int i = 0; i <= hmi.maxIdx; i++) {
        partInfo* current = hmi.A[i];
        while (current != NULL) {
            freeSize += powOf2(i);
            numParts++;
            current = current->nextPart;
        }
    }

    printf("\nHeap Usage Statistics:\n");
    printf("======================\n");

   //Remember to preserve the message format!

    printf("Total Space: %d bytes\n", hmi.totalSize);
    
    printf("Total Free Partitions: %d\n", numParts);
    printf("Total Free Size: %d bytes\n", freeSize);

    printf("Total Internal Fragmentation: %d bytes\n", hmi.internalFragTotal);
}

// Inserts a partition at a specific level
// Partition is inserted according to offset
void insertPartition(partInfo* part, unsigned int lvl) {
        partInfo* current = hmi.A[lvl];
        partInfo* prev = NULL;
        
        printf("Adding partition[+  %d] at Level [%d]\n", part->offset, lvl);

        while (current != NULL && current->offset < part->offset) {
            prev = current;
            current = current->nextPart;
        }
 
        if (prev == NULL) {
            part->nextPart = hmi.A[lvl];
            hmi.A[lvl] = part;
        } else {
            part->nextPart = prev->nextPart;
            prev->nextPart = part;
        }
}

void addPartitionAtLevel( unsigned int lvl, unsigned int offset )
/**********************************************************
 * There is just a suggested approach. You are free NOT to use this.
 *    This function adds a new free partition with "offset" at hmi.A[lvl]
 *    If buddy is found, recursively (or repeatedly) perform merging and insert
 *      at higher level
 *********************************************************/
{
    if(lvl < minLvl) {
        addPartitionAtLevel(minLvl, offset); 
    }

    int buddyOffset = buddyOf(offset, lvl);

    partInfo* current = hmi.A[lvl];
    partInfo* prev = NULL;
    while (current != NULL && current->offset != buddyOffset) {
        prev = current;
        current = current->nextPart;
    }

    if (current == NULL || lvl == maxLvl) {
        insertPartition(buildPartitionInfo(offset), lvl);
    } else {
        if (prev == NULL) {
            hmi.A[lvl] = current->nextPart;
        } else {
            prev->nextPart = current->nextPart;
        }
        
        free(current);

        offset = buddyOffset < offset ? buddyOffset : offset;
        addPartitionAtLevel(lvl + 1, offset);
    }
}

partInfo* removePartitionAtLevel(unsigned int lvl)
/**********************************************************
 * There is just a suggested approach. You are free NOT to use this.
 *    This function remove a free partition at hmi.A[lvl]
 *    Perform the "upstream" search if this lvl is empty AND perform
 *      the repeated split from higher level back to this level.
 * 
 * Return NULL if cannot find such partition (i.e. cannot sastify request)
 * Return the Partition Structure if found.
 *********************************************************/
{

    if (lvl > maxLvl) {
        return NULL;
    }

    if (lvl < minLvl) {
        return removePartitionAtLevel(lvl + 1);
    }

    if (hmi.A[lvl] != NULL) {
        // remove partition
        partInfo* temp = hmi.A[lvl];
        hmi.A[lvl] = temp->nextPart;
        return temp;
    }
    
    partInfo* upstreamPart = removePartitionAtLevel(lvl + 1);

    if (upstreamPart == NULL) {
        return NULL;
    }
   
    int upstreamOffset = upstreamPart->offset;
    free(upstreamPart);
    partInfo* part1 = buildPartitionInfo(upstreamOffset);
    int buddyOffset = buddyOf(upstreamOffset, lvl);
    partInfo* part2 = buildPartitionInfo(buddyOffset);
    insertPartition(part2, lvl);

    return part1;
}

int setupHeap(int initialSize, int minPartSize, int maxPartSize)
/**********************************************************
 * Setup a heap with "initialSize" bytes
 *********************************************************/
{
	void* base;

	base = sbrk(0);
	if(	sbrk(initialSize) == (void*)-1){
		printf("Cannot set break! Behavior undefined!\n");
		return 0;
	}

    hmi.base = base;

	hmi.totalSize = initialSize;
    hmi.internalFragTotal = 0;
	
    //TODO: Task 1. Setup the rest of the bookkeeping info:
    //       hmi.A <= an array of partition linked list
    //       hmi.maxIdx <= the largest index for hmi.A[]
    
    int numLevels = log2Floor(initialSize) + 1;
    
    int size =initialSize;
    minLvl = log2Ceiling(minPartSize);
    maxLvl = log2Floor(maxPartSize);

    hmi.A = (partInfo**) malloc(sizeof(partInfo*) * numLevels);
    hmi.maxIdx = numLevels - 1;
    hmi.maxIdx = hmi.maxIdx > maxLvl ? maxLvl : hmi.maxIdx;

    for (int i = 0; i <= hmi.maxIdx; i++) {
        hmi.A[i] = (partInfo*) NULL;
    }

    while (size > 0) {
        int nextLvl = log2Floor(size);

        if (nextLvl < minLvl) {
            break;
        }
        
        nextLvl = nextLvl > maxLvl ? maxLvl : nextLvl;
        partInfo* part = buildPartitionInfo(initialSize - size);
        insertPartition(part, nextLvl);
        size -= powOf2(nextLvl);
    }

    return 1;
}


void* mymalloc(int size)
/**********************************************************
 * Mimic the normal "malloc()":
 *    Attempt to allocate a piece of free heap of (size) bytes
 *    Return the memory addres of this free memory if successful
 *    Return NULL otherwise 
 *********************************************************/
{
    //TODO: Task 2. Implement the allocation using buddy allocator
    int partLvl = log2Ceiling(size);
    partInfo* part = removePartitionAtLevel(partLvl);
    
    if (part == NULL) {
        return NULL;
    } 
    
    int partOffset = part->offset;
    free(part);

    hmi.internalFragTotal += powOf2(partLvl) - size;
    return (void*)hmi.base + partOffset;
}

void myfree(void* address, int size)
/**********************************************************
 * Mimic the normal "free()":
 *    Attempt to free a previously allocated memory space
 *********************************************************/
{
    //TODO: Task 3. Implement the de allocation using buddy allocator

    // assumption: size can't be greater than max allocatable partition
    int partLvl = log2Ceiling(size);
    
    hmi.internalFragTotal -= powOf2(partLvl) - size;
    int offset = address - hmi.base;
    addPartitionAtLevel(partLvl, offset);
}
