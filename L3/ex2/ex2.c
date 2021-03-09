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
    pthread_mutex_init(&(lock->roomEmpty), NULL);
    // init values: 
    lock->reader_count = 0;
    lock->writer_count = 0;
    pthread_mutex_unlock(&(lock->mutex)); // allows counters to be modified by reader/writer
    pthread_mutex_unlock(&(lock->roomEmpty));
}

void writer_acquire(rw_lock* lock)
{
    //TODO: modify as needed
    // check if room is empty
    pthread_mutex_lock(&(lock->roomEmpty));
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
    pthread_mutex_unlock(&(lock->roomEmpty)); // unlock only when releasing that mutex
}

void reader_acquire(rw_lock* lock)
{
    //TODO: modify as needed
    pthread_mutex_lock(&(lock->mutex));
    lock->reader_count++;
    if(lock->reader_count == 1) {
        pthread_mutex_lock(&(lock->roomEmpty)); // only the first reader needs to lock the roomEmpty
    }
    pthread_mutex_unlock(&(lock->mutex));
}

void reader_release(rw_lock* lock)
{
    //TODO: modify as needed
    pthread_mutex_lock(&(lock->mutex));
    lock->reader_count--;
    if (lock->reader_count == 0) { // release only if it's the last reader:
        pthread_mutex_unlock(&(lock->roomEmpty));
    }
    pthread_mutex_unlock(&(lock->mutex));

}

void cleanup(rw_lock* lock)
{
    //TODO: modify as needed
    pthread_mutex_destroy(&(lock->mutex));
    pthread_mutex_destroy(&(lock->roomEmpty));
}
