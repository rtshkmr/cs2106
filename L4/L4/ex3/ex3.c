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
#include <assert.h>
#include "mmalloc.h"


/**********************************************************
 * This is a "global" structure storing heap information
 * The "static" modifier restrict the structure to be
 *  visible only to functions in this file
 *********************************************************/
static heapMetaInfo hmi;


/**********************************************************
 * Quality of life helper functions / macros
 *********************************************************/
#define powOf2(E) (1 << E)

unsigned int log2Ceiling( unsigned int N )
    /**********************************************************
     * Find the smallest S, such that 2^S >= N
     * S is returned
     *********************************************************/
    // TODO: use this for allocation algo
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

    printf("\nHeap Usage Statistics:\n");
    printf("======================\n");

    printf("Total Space: %d bytes\n", hmi.totalSize);

    //Remember to preserve the message format!

    printf("Total Space: %d bytes\n", hmi.totalSize);

    printf("Total Free Partitions: %d\n", 0);
    printf("Total Free Size: %d bytes\n", 0);

    printf("Total Internal Fragmentation: %d bytes\n", 0);
}

void addPartitionAtLevel( unsigned int lvl, unsigned int offset )
    /**********************************************************
     *    This function adds a new free partition with "offset" at hmi.A[lvl]
     *    If buddy is found, recursively (or repeatedly) perform merging and insert
     *      at higher level
     *********************************************************/
{
    unsigned int buddyOffset = buddyOf(offset, lvl); 
    partInfo* prev = NULL;

    // look for free buddy and bubble up if possible:
    for(partInfo* current = hmi.A[lvl]; current != NULL; prev = current, current = current->nextPart) { // linear search for buddy: 
        if(current && current->offset == buddyOffset) { // found the buddy in the free list: 
            // printf(">>> found a free buddy to merge with. Current level is %d and we shall merge tgt and add ourselves to level %d \n",lvl, lvl + 1);
            unsigned int smallerOffsetValue = (current->offset < offset) ? current->offset : offset; // choose the smaller of the two
            // remove current from this list:
            if(!prev) {
                hmi.A[lvl] = current->nextPart;
            } else prev->nextPart = current->nextPart;
            free(current);
            return addPartitionAtLevel(lvl + 1, smallerOffsetValue);
        }
        // prev = current; 
    }

    // buddy is busy, add new partition to this level: 
    partInfo* newPartition = buildPartitionInfo(offset); 
    newPartition->nextPart = hmi.A[lvl];
    hmi.A[lvl] = newPartition;
    return;
}

partInfo* removePartitionAtLevel(unsigned int lvl)
    /**********************************************************
     *    This function removes a free partition at hmi.A[lvl]
     *    Perform the "upstream" search if this lvl is empty AND perform
     *    the repeated split from higher level back to this level.
     * 
     * Return NULL if cannot find such partition (i.e. cannot sastify request)
     * Return the Partition Structure if found.
     *********************************************************/
{
    // level is the idx to look at
    partInfo* candidate = hmi.A[lvl];
    if(candidate == NULL) { // look above and add 
        partInfo* largerTarget  = NULL;
        for(int i = lvl + 1;i <= hmi.maxIdx; i++) {
            if(hmi.A[i] != NULL) {
                largerTarget = hmi.A[i];
                hmi.A[i] = largerTarget->nextPart;
                // split larger into two buddy partitions and add it to the front of the linked list:
                int newLevel = i - 1;
                unsigned int selfOffset = largerTarget->offset; //
                unsigned int buddyOffset = buddyOf(selfOffset, newLevel); // TODO: isn't buddyOf's param supp to be addr and not offset? 
                partInfo* self = buildPartitionInfo(selfOffset); // unnecessary step, can just reasign ptrs
                partInfo* buddy = buildPartitionInfo(buddyOffset);
                self->nextPart = buddy;
                buddy->nextPart = hmi.A[newLevel];
                hmi.A[newLevel] = self;
                break;
            }
        }
        if(largerTarget == NULL) {
            return NULL;
        } else { 
            free(largerTarget); 
            return removePartitionAtLevel(lvl);
        }
    } else {
        // found an empty, allocatable partition:
        hmi.A[lvl] = candidate->nextPart;
        return candidate;
    }
}

int setupHeap(int initialSize)
    /**********************************************************
     * Setup a heap with "initialSize" bytes
     *********************************************************/
{
    void* base;

    base = sbrk(0); // this helps (set and) find the current location of the program break
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

    // Figure out the num of levels via continuous Log2:
    int numLevels = 0;
    while(initialSize != 0) {
        numLevels++;
        initialSize = initialSize >> 1; // right shift 1 bit
    }

    // allocate space for numLevels levels of partInfo arrays
    hmi.A = (partInfo**) malloc(sizeof(partInfo*) * numLevels);   //change this!
    partInfo* initialFreePartition = buildPartitionInfo(0);
    // partInfo* initialFreePartition = (partInfo*) malloc(sizeof(partInfo));
    initialFreePartition->offset = 0;
    initialFreePartition->nextPart = (partInfo*) NULL;
    hmi.maxIdx = numLevels - 1; //change this!
    hmi.A[numLevels - 1] = initialFreePartition; // highestIdx will be allocated first 
    for(int i = 0; i < numLevels - 1; i++) {
        hmi.A[i] = (partInfo*) NULL;
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
    printf(">>> mymalloc(%d)\n", size);
    partInfo* target = removePartitionAtLevel(log2Ceiling(size));
    void* memAddr = hmi.base + target->offset; 
    if(!target) {
        free(target);
        return NULL;
    } else {
        free(target);
        return memAddr;
    }
}

void myfree(void* address, int size)
    /**********************************************************
     * Mimic the normal "free()":
     *    Attempt to free a previously allocated memory space
     *********************************************************/
{
    //TODO: Task 3. Implement the de allocation using buddy allocator
    // int offset = address - hmi.base;
    // printf(">>> myfree(%d,%d).\n", offset , size );

    // add a partition to the hmi array to indicate it as free: 
    addPartitionAtLevel(log2Ceiling(size), address - hmi.base);
}
