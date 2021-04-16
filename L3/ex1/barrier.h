/*************************************
* Lab 3 Exercise 1
* Name:
* Student Id: A0201829H
* Lab Group: B12
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

/**
 * CS2106 AY 20/21 Semester 2 - Lab 3
 *
 * This file contains declarations. You should only modify the barrier_t struct,
 * as the method signatures will be needed to compile with the runner.
 */
#ifndef __CS2106_BARRIER_H_
#define __CS2106_BARRIER_H_
#include <semaphore.h>

typedef struct barrier {
  int count;
  //TODO: add additional fields here
  // semaphore pointers are kept local to the barrier since  the barrier is one-time use
  sem_t *waitQ;
  sem_t *mutex;
} barrier_t;

void barrier_init ( barrier_t *barrier, int count );
void barrier_wait ( barrier_t *barrier );
void barrier_destroy ( barrier_t *barrier );

#endif // __CS2106_BARRIER_H_
