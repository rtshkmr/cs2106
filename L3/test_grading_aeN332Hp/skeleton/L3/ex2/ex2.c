/*************************************
* Lab 3 Exercise 2
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_init(&(lock->mutex), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->mutex));
  lock->writer_count++;
  pthread_mutex_unlock(&(lock->mutex));
}

void writer_release(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->mutex));
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->mutex));
}

void reader_acquire(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->mutex));
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->mutex));
}

void reader_release(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->mutex));
  lock->reader_count--;
  pthread_mutex_unlock(&(lock->mutex));
}

void cleanup(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_destroy(&(lock->mutex));
}