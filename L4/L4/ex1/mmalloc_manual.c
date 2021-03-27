#include <stdio.h>
#include <stdlib.h>
#include "mmalloc.h"

/**********************************************************
 * There is no need to modify this file for this exercise
 *********************************************************/
/**********************************************************
 * A manual test driver, essentially prompting user for 
 *  information.
 * Useful for more targetted test as the randomized tester
 *  is hard to check certain condition.
 * Can be compiled with DEBUG flag to enable verbose trace
 *********************************************************/



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


int allocate(allocateInfo allocated[], int pos, int nInt)
/**********************************************************
 * Perform allocation and store the allocation result at allocated[pos]
 * 
 * Return 1 (true) if allocation is succcessful
 * Return 0 (false) otherwise
 *********************************************************/
{
    int *address, size;

    size = sizeof(int) * nInt;
#ifdef DEBUG
    printf("A: %d integers = %d bytes\n", nInt, size);
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

int deallocate(allocateInfo allocated[], int size, int pos)
/**********************************************************
 * Perform de-allocation (free) using an earlier
 *  allocation result at allocated[pos]
 * 
 * Return the new size of the allocated[] array. 
 *********************************************************/
{
    int i;

#ifdef DEBUG
    printf("F: %p [S=%ld]\n", allocated[pos].address,
        allocated[pos].nInt*sizeof(int));
#endif

    //Invalid position
    if (pos >= size ){
        return size;
    }

    //Check for values in the memory region
    //Incorrect checks indicates a potential wrong allocation earlier
    if (!checkValues( allocated[pos].address, 
                        allocated[pos].nInt)){
        printf("Memory corruption detected!\n");
        printf("Memory region at [%p] should have %d "
               "magic values\n", 
                allocated[pos].address,
                allocated[pos].nInt);
        exit(1);
    }

    //Using our own free() implementation
    myfree( (void*) allocated[pos].address);

    //Shift to close gap
    for (i = pos; i < size - 1; i++){
        allocated[i] = allocated[i+1];
    } 

    return size-1;
}

void printMenu()
/**********************************************************
 * Print a simple menu for supported funtionalities
 *********************************************************/
{
    printf("\n********* Menu *********\n");
    printf("Allocate= 1 <nInt>  | Deallocate = 2        |\n");
    printf("Print Heap Meta = 3 | Print Heap Stat = 4 |\n");
    printf("Print Entire Heap = 5 | Exit = 0\n");
    printf("Your Choice => ");
}


void manualTester(int nReq)
/**********************************************************
 * Manual Test Driver
 *********************************************************/
{
    int i;
 
    int nAllocated;
    allocateInfo *allocated;

    int userInput, userArg;

    allocated = (allocateInfo*) malloc(sizeof(allocateInfo) * nReq);
    for (i = 0; i < nReq; i++){
        allocated[i].address = NULL;
    }

    nAllocated = 0;
    do {
        printMenu();
        scanf("%d", &userInput);
        switch (userInput){
            case 1:
                scanf("%d", &userArg);

                if (allocate(allocated, nAllocated, userArg)){
                    writeValues(allocated[nAllocated].address, 
                                allocated[nAllocated].nInt);
                    printf("A: Allocated at [%p], request idx [%d]\n",
                            allocated[nAllocated].address, nAllocated);
                    nAllocated++;
                } else {
                    printf("A: FAILED\n");
                }
                break;
            case 2:
                printf("Existing partitions:\n");
                printAllocateInfo(allocated, nAllocated);
                printf("Free partition <idx> = ");
                scanf("%d", &userArg);

                nAllocated = deallocate(allocated, nAllocated, userArg);
                break;
            case 3:
                    printHeapMetaInfo();
                    break;
            case 4:
                    printHeapStatistic();
                    break;
            case 5:
                    printHeap();
                    break;
        }
    } while (userInput != 0);


    //Clean up

#ifdef DEBUG
    printf("*Testing Done. Cleaning up!\n");
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

int main(int argc, char** argv)
{
    int nReq, heapSize;
  
    if (argc != 3){
        printf("Usage: %s <Heap Size> <Max Valid Partitions#>\n",argv[0]);
        return 1;
    }

    heapSize = atoi(argv[1]);
    nReq = atoi(argv[2]);

#ifdef DEBUG
    printf("Heap size %d, #Req %d\n", heapSize, nReq);
#endif
    
    //Initialize our "heap"
    if(!setupHeap(heapSize)){
        printf("Heap setup failed. Exiting.\n");
        return 1;
    }

    manualTester(nReq);

    printHeapMetaInfo();
    printHeapStatistic();

    return 0;
}
