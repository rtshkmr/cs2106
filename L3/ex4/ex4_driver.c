/*************************************
 * Lab 3 Exercise 4
 *************************************
You may change this file during your own testing, 
but note that they will be replaced with the original 
files when we test your assignments.
 *************************************/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "traffic_synchronizer.h"

#define ensure_successful_malloc(ptr)                           \
    if (ptr == NULL) {                                          \
        printf("Memory allocation unsuccessful for" #ptr "\n"); \
        exit(1);                                                \
    }

int   num_of_segments;
int   cars_per_street;
int   num_of_cars;
segment_struct *segments;

int   concurrently_moving_cars;
int   concurrently_moving_cars_max;
sem_t concurrently_moving_cars_mutex;

int   concurrent_cars;
int   concurrent_cars_max;
sem_t concurrent_cars_mutex;

int main(int argc, char** argv)
{
    if (argc < 4) {
        printf("Usage: %s random_seed number_of_streets cars_per_street\n", argv[0]);
        exit(1);
    }
    car_struct *cars;
    

    int seed = atoi(argv[1]);
    num_of_segments = atoi(argv[2]);
    cars_per_street = atoi(argv[3]);
    num_of_cars = num_of_segments * cars_per_street;

    srand(seed);
    
    
    
    concurrently_moving_cars_max = concurrently_moving_cars = 0;
    sem_init(&concurrently_moving_cars_mutex, 0, 1);
    
    concurrent_cars_max = concurrent_cars = 0;
    sem_init(&concurrent_cars_mutex, 0, 1);
    
    cars = malloc (sizeof (car_struct) * num_of_cars);
    ensure_successful_malloc(cars);
    
    segments = malloc (sizeof (segment_struct) * num_of_segments);
    ensure_successful_malloc (segments);
    
    int i;
    
    //initialize the cars
    for (i = 0; i < num_of_cars; i++) {
        cars[i].car_id = i;
        cars[i].entry_seg = i/cars_per_street;
        do {        
            cars[i].exit_seg = rand() % num_of_segments;
        } while  (cars[i].exit_seg ==  cars[i].entry_seg);    
        cars[i].current_seg = -1;
        

    }
    for (i = 0; i < num_of_segments; i++) {
        sem_init(&segments[i].cars_in_seg_mutex, 0, 1);
    }
        
    initialise();

    pthread_t car_threads[num_of_cars];

    for (i = 0; i < num_of_cars; i++) {
        pthread_create(&car_threads[i], NULL, car, (void*)&cars[i]);
    }

    for (i = 0; i < num_of_cars; i++) {
        pthread_join(car_threads[i], NULL);
    }

    cleanup();
    

   

    sem_destroy(&concurrently_moving_cars_mutex);
    sem_destroy(&concurrent_cars_mutex);
    for (i = 0; i < num_of_segments; i++) {
        sem_destroy(&(segments[i].cars_in_seg_mutex));
    }
    printf("Maximum number of cars moving: %d\n", concurrently_moving_cars_max);
    printf("Maximum number of cars in the roundabout: %d\n", concurrent_cars_max);
    free (cars);
    free (segments);
    pthread_exit(0);
}

void print_roundabout_state()
{
    int i;
    printf("Roundabout state: ");
    for (i = 0; i < num_of_segments; i++) {
        printf("[%d]: %d | ", i, segments[i].cars_in_seg);
    }
    printf("\n");
}

void check_move (int roundabout_seg)
{
    int num_cars = segments[roundabout_seg].cars_in_seg;
    if (num_cars != 1) {
#ifdef DEBUG
        print_roundabout_state();
#endif
        printf("Crash at roundabout pos: %d, %d cars found\n", roundabout_seg, num_cars);
        exit(1);
    }
}

void increment_cars_count()
{
    sem_wait(&concurrent_cars_mutex);
    if (++concurrent_cars > concurrent_cars_max) {
        concurrent_cars_max = concurrent_cars;
    }
    sem_post(&concurrent_cars_mutex);
}

void decrement_cars_count()
{
    sem_wait(&concurrent_cars_mutex);
    concurrent_cars--;
    sem_post(&concurrent_cars_mutex);
}

void increment_moving_cars_count()
{
    sem_wait(&concurrently_moving_cars_mutex);
    if (++concurrently_moving_cars > concurrently_moving_cars_max) {
        concurrently_moving_cars_max = concurrently_moving_cars;
    }
    sem_post(&concurrently_moving_cars_mutex);
}

void decrement_moving_cars_count()
{
    sem_wait(&concurrently_moving_cars_mutex);
    concurrently_moving_cars--;
    sem_post(&concurrently_moving_cars_mutex);
}

//function should be called from car thread to move the car from the street to the entry segment
void enter_roundabout(car_struct* car)
{
    int entry_no = car->entry_seg;
    sem_wait(&segments[entry_no].cars_in_seg_mutex);
    segments[entry_no].cars_in_seg ++;

    check_move (entry_no);    
    car->current_seg = entry_no;
#ifdef DEBUG
    printf ("Car [%d]: entered to %d\n", car->car_id, car->current_seg);
    print_roundabout_state();
#endif
    sem_post(&segments[entry_no].cars_in_seg_mutex);
    
    increment_cars_count();

}

//function should be called from car thread to move the car from the exit segment out on the steet
void exit_roundabout(car_struct* car)
{
    int exit_no = car->exit_seg;
    if (exit_no != car->current_seg){
#ifdef DEBUG
        print_roundabout_state();
#endif
        printf("Mismatch exit number for car %d. Trying to exit at segment %d instead of %d \n", 
            car->car_id, car->current_seg, car->exit_seg);
        exit(1);
    }
    sem_wait(&segments[exit_no].cars_in_seg_mutex);
    segments[exit_no].cars_in_seg --;   
#ifdef DEBUG
    printf ("Car [%d]: exited from %d\n", car->car_id, car->current_seg);
#endif
    sem_post(&segments[exit_no].cars_in_seg_mutex);
    decrement_cars_count();
}


//function should be called from car thread to move a car from current position to NEXT segment
void move_to_next_segment(car_struct* car)
{
    int seg_no = car->current_seg;
    increment_moving_cars_count();
    
    car->current_seg = NEXT(seg_no, num_of_segments);
    
    
    sem_wait(&segments[seg_no].cars_in_seg_mutex);
    segments[seg_no].cars_in_seg--;
    sem_post(&segments[seg_no].cars_in_seg_mutex);

    sem_wait(&segments[car->current_seg].cars_in_seg_mutex);
    segments[car->current_seg].cars_in_seg++; 
    check_move (car->current_seg);
    sem_post(&segments[car->current_seg].cars_in_seg_mutex);       
    
    usleep (100);
#ifdef DEBUG
    printf ("Car [%d]: moved to %d\n", car->car_id, car->current_seg);
    print_roundabout_state();
#endif

    decrement_moving_cars_count();
}




