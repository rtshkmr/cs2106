/*************************************
 * Lab 3 Exercise 4
 *************************************
You may change this file during your own testing, 
but note that they will be replaced with the original 
files when we test your assignments.
 *************************************/
 
#include <semaphore.h>

typedef struct
{
    int car_id;
    int entry_seg;
    int exit_seg;
    int current_seg;
} car_struct;



typedef struct
{
    sem_t cars_in_seg_mutex;
    int cars_in_seg;
} segment_struct;


// Some helpful macros for the program
#define PREV(x, n) ((x + n - 1) % n)
#define NEXT(x, n) ((x + 1) % n)

void initialise();
void cleanup();
void* car (void *car);


void enter_roundabout(car_struct* car);
void exit_roundabout(car_struct* car);
void move_to_next_segment(car_struct* car);

