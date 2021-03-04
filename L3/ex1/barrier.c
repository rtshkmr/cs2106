/*************************************
* Lab 3 Exercise 1
* Name:
* Student Id: A????????
* Lab Group: B??
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

// Initialise barrier here
void barrier_init ( barrier_t *barrier, int count ) 
{
    //TODO: Implement the function
    barrier->count = count;
}

void barrier_wait ( barrier_t *barrier ) 
{
    //TODO: Implement the function
}

// Perform cleanup here if you need to
void barrier_destroy ( barrier_t *barrier ) 
{
    //TODO: Implement the function
}
