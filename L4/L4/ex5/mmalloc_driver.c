#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mmalloc.h"

/**********************************************************
 * There is no need to modify this file for this exercise
 *********************************************************/
/**********************************************************
 * An auto, randomized multithreaded tester to stress-test mymalloc()
 *  and myfree() functions, focusing on thread-safe implementation
 * Can be compiled with DEBUG flag to enable verbose trace
 *********************************************************/


//This is a flag set by mymalloc() and myfree() if race condition
// is detected
extern int memOpIntegrity;

/**********************************************************
 * A simple structure to keep information of a successful mymalloc()
 *********************************************************/
typedef struct ainfo {
    int* address;
    int nInt;
} allocateInfo;

void printAllocateInfo(allocateInfo allocated[], int size)
/**********************************************************
 * Print the successful mymalloc() so far.
 *********************************************************/
{
    int i;

    for (i = 0; i < size; i++){
        printf("%4d | Addr %8p | nInt %d\n",
                i, allocated[i].address, allocated[i].nInt);
    }

}

void writeValues(int array[], int size)
/**********************************************************
 * Fill the entire array with a magic number.
 * For simplicity, the "size" is also used as the magic number.
 *********************************************************/
{
    //can be achieved with memcpy() too

    int i;

    //We use the size as the "magic number"
    for(i = 0; i < size; i++){
        array[i] = size;
    }
}

int checkValues(int array[], int size)
/**********************************************************
 * Verify whether the entire range has the same magic number
 * For simplicity, the "size" is also used as the magic number
 *********************************************************/
{


    int i;

    for(i = 0; i < size; i++){
        if( array[i] != size ){
            return 0;
        }
    }

    return 1;
}

/**********************************************************
 * Tunable parameters for random allocation
 * Minimally allocate 10 integers (i.e. 40 bytes)
 * Maximally allocate 100 integers (i.e. 400 bytes)
 *********************************************************/
#define MINALLOCATE 10 
#define MAXALLOCATE 100 


int randomAllocate(allocateInfo allocated[], int pos)
/**********************************************************
 * Perform random allocation.
 * Store the random allocation result at allocated[pos]
 * 
 * Return 1 (true) if allocation is succcessful
 * Return 0 (false) otherwise
 *********************************************************/
{
    int nInt;
    int *address;

    nInt = (rand() % (MAXALLOCATE - MINALLOCATE)) + MINALLOCATE;

#ifdef DEBUG
    printf("A: %d integers\n", nInt);
#endif

    //Using our own malloc() implementation
    address = (int*) mymalloc(sizeof(int) * nInt);

    if (address == NULL){
        return 0;
    }

    allocated[pos].address = address;
    allocated[pos].nInt = nInt;

    return 1;
}

int randomFree(allocateInfo allocated[], int size)
/**********************************************************
 * Perform random de-allocation (free).
 * Pick one of the earlier successful allocation and free it.
 * 
 * Return the new size of the allocated[] array. 
 *********************************************************/
{
    int rNum, i;

    if (size == 0){
        return size;
    }

    rNum = rand() % size;

#ifdef DEBUG
    printf("F: %p [S=%d, R=%d]\n", allocated[rNum].address, size, rNum);
#endif

    //Check for values in the memory region
    //Incorrect checks indicates a potential wrong allocation earlier
    if (!checkValues( allocated[rNum].address, 
                        allocated[rNum].nInt)){
        printf("Memory corruption detected!\n");
        printf("Memory region at [%p] should have %d "
               "magic values\n", 
                allocated[rNum].address,
                allocated[rNum].nInt);
        exit(1);
    }

    //Using our own free() implementation
    myfree( (void*) allocated[rNum].address );

    //Shift to close gap
    for (i = rNum; i < size - 1; i++){
        allocated[i] = allocated[i+1];
    } 

    return size-1;
}

void tester(int nReq)
/**********************************************************
 * A automated, randomized tester
 * Perform "nReq" memory requests
 * 
 * To have more meaningful operations, we first perform 10%
 *  of memory allocation (mymalloc())
 * For the remaining 90% of requests, we will attempt 55% malloc and 45% free
 *********************************************************/
{
    int i, startPhase, rNum;
 
    int nAllocated;
    allocateInfo *allocated;

    allocated = (allocateInfo*) malloc(sizeof(allocateInfo) * nReq);
    for (i = 0; i < nReq; i++){
        allocated[i].address = NULL;
    }

    startPhase = nReq / 10;  //The starting 10% are all malloc request

    //Start Phase = all malloc
    nAllocated = 0;
    for (i = 0; i < startPhase; i++){

        if (randomAllocate(allocated, nAllocated)){
            writeValues(allocated[nAllocated].address, 
                        allocated[nAllocated].nInt);
            nAllocated++;
#ifdef DEBUG
    printAllocateInfo(allocated, nAllocated);  
    printHeapMetaInfo();
#endif
        }
    }

    //Remainder of requests = 55% malloc, 45% free
    for (; i < nReq; i++){
        rNum = rand() % 100;

        if (rNum < 55 ){
            if (randomAllocate(allocated, nAllocated)){
                writeValues(allocated[nAllocated].address, 
                        allocated[nAllocated].nInt);
                nAllocated++;
            }
        } else {
            
            nAllocated = randomFree(allocated, nAllocated);
        } 

#ifdef DEBUG
    printAllocateInfo(allocated, nAllocated);  
    printHeapMetaInfo();
#endif
    }


    //Clean up

#ifdef DEBUG
    printf("*Tester Done. Cleaning up!\n");
#endif
    //Technically we are supposed to clear each of the allocated 
    // memory space
    // However, that'd destroy the final heap statistics, so we
    // leave those allocation behind
    /*
    for(i = 0; i < nAllocated; i++){
        if (allocated[i] != NULL){
            myfree(allocated[i]);
        }
    }
    */

    //Need to return the array back to system
    free(allocated);
}

void* wrapper(void* arg)
/**********************************************************
 * A wrapper for thread start function, call tester()
 *********************************************************/

{
    //Typecast gymnastic to get the integer argument
    int nReq = *((int*)arg);

    printf("Tester Thread[%ld] with [%d] requests\n",
        pthread_self(), nReq);

    tester(nReq);

    printf("Tester Thread[%ld] done!\n", pthread_self());

    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    int seed, nReq, heapSize, nThread, i;

    pthread_t* tidArray;
  
    if (argc < 5){
        printf("Usage: %s <Random Seed> <Heap Size> <Requests#> <Thread#>\n",argv[0]);
        return 1;
    }

    seed = atoi(argv[1]);
    heapSize = atoi(argv[2]);
    nReq = atoi(argv[3]);
    nThread = atoi(argv[4]);

#ifdef DEBUG
    printf("Seed %d, heap size %d, #Req %d, #Thread %d\n", seed, heapSize, nReq, nThread);
#endif
    
    //Setup Random Seed
    srand(seed);
 
    //Initialize our "heap"
    if(!setupHeap( heapSize )){
        return 1;
    }

#ifdef DEBUG
    printHeapMetaInfo();
#endif

    //Setup threads
    tidArray = (pthread_t*) malloc(sizeof(pthread_t) * nThread );

    //Start testing
    for ( i = 0; i < nThread; i++){
        pthread_create(&tidArray[i], NULL, wrapper, (void*)&nReq); 
    }
    
    //Wait for threads
    for ( i = 0; i < nThread; i++){
        pthread_join(tidArray[i], NULL);
    }

#ifdef DEBUG
    printHeap();  //Essentially do a "core-dump"
#endif

    printHeapMetaInfo();

    //Report Race Condition status
    if (memOpIntegrity == 1){
        printf("** Memory operation is thread-safe\n");
    } else {
        printf("** FAILED: Memory operation is NOT thread-safe\n");
    }
    
    free(tidArray);

    return 0;
}
