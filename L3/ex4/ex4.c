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

sem_t *segment_locks; //Array of semaphores, one for each segment
sem_t loop_lock;


void initialise()
{
    //TODO: Your code here
    
    segment_locks = malloc(sizeof(sem_t) * num_of_segments);
    for (int i = 0; i < num_of_segments; i++) {
        sem_init(&segment_locks[i], 0, 1);
    }
    sem_init(&loop_lock, 0, num_of_segments - 1); // -1 to prevent scenario when cars cannot move as all segments are filled

}

void cleanup()
{
    //TODO: Your code here
    for (int i = 0; i < num_of_segments; i++) {
        sem_destroy(&segment_locks[i]);
    }
    sem_destroy(&loop_lock);
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
    car_struct* carObj = (car_struct*) car;
    
    int exit_seg = carObj->exit_seg;

    // Enter Roundabout
    sem_wait(&loop_lock);
    sem_wait(&segment_locks[carObj->entry_seg]);
    enter_roundabout(car);
    
    // Move to next segment
    while (exit_seg != carObj->current_seg) {
        int current_seg = carObj->current_seg;
        sem_wait(&segment_locks[(current_seg + 1) % num_of_segments]); 
        move_to_next_segment(car);
        sem_post(&segment_locks[current_seg]);
    }
   
    // Exit_roundabout
    exit_roundabout(car);
    sem_post(&segment_locks[exit_seg]);
    sem_post(&loop_lock);
    pthread_exit(NULL);
}
