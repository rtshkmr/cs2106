/*************************************
* Lab 3 Exercise 2
* Name: Ritesh Kumar    
* Student Id: A0201829H
* Lab Group: B12
*
*
* Name: Fang Junwei, Samuel
* Student Id: A0199163U
* Lab Group: B01
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/


#ifndef __CS2106_RWLOCKV1_H_
#define __CS2106_RWLOCKV1_H_

#include <pthread.h> // To import the pthread_mutex_t type.

//TODO: You can modify the structure
typedef struct {
  
  pthread_mutex_t in; // indicates interest for room entrance 
  pthread_mutex_t mutex; // controls changing of reader_count
  pthread_mutex_t writer; // controls entry into room
  int reader_count;
  int writer_count;
} rw_lock;

//Note: You are NOT allowed to change the function headers
void initialise(rw_lock* lock);
void writer_acquire(rw_lock* lock);
void writer_release(rw_lock* lock);
void reader_acquire(rw_lock* lock);
void reader_release(rw_lock* lock);
void cleanup(rw_lock* lock);

#endif
