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

segment_struct *segments;

void initialise()
{
    //TODO: Your code here

    for (int i = 0; i < num_of_segments; i++) {
        sem_init(&segments[i].cars_in_seg_mutex, 0, 1);
    }

}

void cleanup()
{
    //TODO: Your code here
    for (int i = 0; i < num_of_segments; i++) {
        sem_destroy(&(segments[i].cars_in_seg_mutex));
    }
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
    
    int curr_seg = carObj->current_seg;

    sem_wait(&segments[carObj->current_seg].cars_in_seg_mutex);
    enter_roundabout(car);


    while (carObj->exit_seg != carObj->current_seg) {
        curr_seg = carObj->current_seg;
        sem_post(&segments[(curr_seg + 1) % num_of_segments].cars_in_seg_mutex); 
        move_to_next_segment(car);
        sem_post(&segments[curr_seg].cars_in_seg_mutex);
    }
    
    curr_seg = carObj->current_seg; 
    exit_roundabout(car);
    sem_post(&segments[curr_seg].cars_in_seg_mutex);
    
    pthread_exit(NULL);
}
