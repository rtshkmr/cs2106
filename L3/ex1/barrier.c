/*************************************
* Lab 3 Exercise 1
* Name: Ritesh Kumar
* Student Id: A0201829H 
* Lab Group: B12
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

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "barrier.h"



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
    // allocate & check space: 
    barrier->mutex = malloc(sizeof(sem_t));
    barrier->waitQ = malloc(sizeof(sem_t));
    ensure_successful_malloc(barrier->mutex); 
    ensure_successful_malloc(barrier->waitQ);

    // set init values for semaphores: 
    sem_init(barrier->mutex, 0, 1 ); // mutex needs to start at init value of 1 
    sem_init(barrier->waitQ, 0, 0 ); // shared semaphore init to waitQ of 0, only last thread will set value of 1 for this
}

void barrier_wait ( barrier_t *barrier ) 
{
    //TODO: Implement the function
    // decrement the count if it's safe to do so
    sem_wait(barrier->mutex);
    barrier->count--;
    sem_post(barrier -> mutex);

    if(barrier->count > 0) { // last person hasn't arrived, still need to wait
        sem_wait(barrier->waitQ);
        sem_post(barrier->waitQ);
    } else { // last one does wait 
        sem_post(barrier->waitQ);
    }
}

// Perform cleanup here if you need to
void barrier_destroy ( barrier_t *barrier ) 
{
    //TODO: Implement the function
    // destroy the semaphores first then free the locations by using the same pointers
    sem_destroy(barrier->mutex); // prevents mem leaks
    sem_destroy(barrier->waitQ);
    free(barrier->mutex); 
    free(barrier->waitQ); 
}
