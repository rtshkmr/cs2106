/*************************************
* Lab 3 Exercise 4
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#include "traffic_synchronizer.h"

//Using extern, you can use the global variables num_of_cars and num_of_segments from 
// ex4_driver.c in your code.
extern int num_of_cars;
extern int num_of_segments;
// additional variables: 
sem_t *segment_locks; // mutex but we just gonna use general sem
sem_t roundabout_lock; // to prevent max number of segments from "trying" at the same time. we limit it to n/2 


void initialise()
{
    //int roundabout_init = num_of_segments / 2; 
    int roundabout_init = num_of_segments - 1; // note: any number between n / 2 and n - 1 inclusive would work as an init value. The testing doesn't seem to make a difference in terms of concurrency
    //TODO: Your code here
    // allocate space for the array of semaphores: 
    segment_locks = malloc(sizeof(sem_t) * num_of_segments);
    for(int i = 0 ; i < num_of_segments; i++) {
        sem_init(&segment_locks[i], 0 , 1);
    }
    sem_init(&roundabout_lock, 0 , roundabout_init); // TODO: epxlore other init values again 
}

void cleanup()
{
    for(int i = 0 ; i < num_of_segments; i++) {
        sem_destroy(&segment_locks[i]);
    }
    sem_destroy(&roundabout_lock);
    free(segment_locks);
}

void* car(void* car)
{
    //TODO: Your code here, see the general steps below

    //This function modeles a thread 
    //A car: 
    //   1. should call enter_roundabout (...)
    //   2. followed by some calls to move_to_next_segment (...)
    //   3. finally call exit_roundabout (...)
    
    // entry: 
    car_struct *carPtr = (car_struct*) car;
    int myEntry = carPtr->entry_seg;
    int myExit = carPtr -> exit_seg;
    // check that the roundabout is safe to enter (safe distancing problem): 
    sem_wait(&roundabout_lock);
    // check if the segment is safe to enter: 
    sem_wait(&segment_locks[myEntry]); 
    // once here, it's guarateed that no one else is in the segment.
    enter_roundabout(carPtr); 


    while(myExit != carPtr->current_seg) {
        // movement from one segment to next: 
        int currSegment = carPtr->current_seg; 
        int nextSegment = NEXT(currSegment, num_of_segments); 
        sem_wait(&segment_locks[nextSegment]);
        // when here, guaranteed no one inside nextSeg 
        move_to_next_segment(carPtr);
        // release lock for the original segment: 
        sem_post(&segment_locks[currSegment]);
    }
    
   // exit: 
   exit_roundabout(carPtr); 
   // safe to unlock segments: 
   sem_post(&segment_locks[myExit]);
   sem_post(&roundabout_lock);
   pthread_exit(NULL);
}
