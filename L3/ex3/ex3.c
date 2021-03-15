/*************************************
* Lab 3 Exercise 3
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

/************************************

You should use ex2 solution as the starting point.

Copy over the solution and modify as needed.
************************************/

#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  pthread_mutex_init(&(lock->turnstile), NULL);
  pthread_mutex_init(&(lock->mutex), NULL);
  pthread_mutex_init(&(lock->writerLock), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->turnstile));
  pthread_mutex_lock(&(lock->writerLock));
  lock->writer_count++;
}

void writer_release(rw_lock* lock)
{
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->turnstile));
  pthread_mutex_unlock(&(lock->writerLock));
}

void reader_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->turnstile));
  pthread_mutex_unlock(&(lock->turnstile));
  pthread_mutex_lock(&(lock->mutex));
  if (lock->reader_count == 0) {
      pthread_mutex_lock(&(lock->writerLock));
  }
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->mutex));
}

void reader_release(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->mutex));
  lock->reader_count--;
  if (lock->reader_count == 0) {
      pthread_mutex_unlock(&(lock->writerLock));
  }
  pthread_mutex_unlock(&(lock->mutex));
}

void cleanup(rw_lock* lock)
{
  pthread_mutex_destroy(&(lock->turnstile));
  pthread_mutex_destroy(&(lock->mutex));
  pthread_mutex_destroy(&(lock->writerLock));
}
