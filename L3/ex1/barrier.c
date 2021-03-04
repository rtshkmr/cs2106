/*************************************
* Lab 3 Exercise 1
* Name: Fang Junwei, Samuel
* Student Id: A0199163U
* Lab Group: B01
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

/**
 * CS2106 AY 20/21 Semester 2 - Lab 3
 *
 * This file contains function definitions. Your implementation should go in
 * this file.
 */

#include "barrier.h"
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define ensure_successful_malloc(ptr)                           \
  if (ptr == NULL) {                                            \
    perror("Memory allocation unsuccessful for" #ptr "\n");     \
    exit(1);                                                    \
  }

// Initialise barrier here
void barrier_init ( barrier_t *barrier, int count ) 
{
    //TODO: Implement the function
    barrier->count = count;

    // Initialise mutex
    barrier->mutex = malloc(sizeof(sem_t));
    ensure_successful_malloc( barrier->mutex );
    sem_init( barrier->mutex, 1, 1 );

    // Initialise block
    barrier->block = malloc(sizeof(sem_t));
    ensure_successful_malloc( barrier->block );
    sem_init( barrier->block, 0, 1 );
}

void barrier_wait ( barrier_t *barrier ) 
{
    //TODO: Implement the function
    sem_wait(barrier->mutex);
    barrier->count--;
    sem_post(barrier->mutex);

    if (barrier->count == 0) {
        sem_post(barrier->block);
    } else {
        sem_wait(barrier->block);
        sem_post(barrier->block);
    }

}

// Perform cleanup here if you need to
void barrier_destroy ( barrier_t *barrier ) 
{
    //TODO: Implement the function
    sem_destroy( barrier->mutex );
    free( barrier->mutex );
    sem_destroy( barrier->block );
    free( barrier->block );
}
