/*************************************
 * Lab 3 Exercise 2
 * Name:
 * Student Id: A????????
 * Lab Group: B??
 *************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "rw_lock.h"
#include <stdio.h>

void initialise(rw_lock* lock)
{
    printf("initializeeeeee");
    //TODO: modify as needed
    pthread_mutex_init(&(lock->in), NULL);
    pthread_mutex_init(&(lock->mutex), NULL);
    pthread_mutex_init(&(lock->writer), NULL);
    // init values: 
    lock->reader_count = 0;
    lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
    printf("writer_acquire");
    pthread_mutex_lock(&(lock->in)); // indicate interest to enter room 
    // check if room is empty
    pthread_mutex_lock(&(lock->writer)); // writer enters the room
    lock->writer_count++;
}

void writer_release(rw_lock* lock)
{
    printf("writer_release");
    lock->writer_count--;
    pthread_mutex_unlock(&(lock->in)); // no more interest in entering the room
    pthread_mutex_unlock(&(lock->writer)); // unlock only when releasing that mutex
}

void reader_acquire(rw_lock* lock)
{
    printf("reader_acquire");
    pthread_mutex_lock(&(lock->in)); // wait/indicate interest to enter
    pthread_mutex_lock(&(lock->mutex));
    if(lock->reader_count == 0) {
        pthread_mutex_lock(&(lock->writer)); // only the first reader needs to lock the roomEmpty
    }
    lock->reader_count++;
    pthread_mutex_unlock(&(lock->mutex));
    pthread_mutex_unlock(&(lock->in)); // others can show interest now
}

void reader_release(rw_lock* lock)
{
    printf("reader_release");
    //TODO: modify as needed
    pthread_mutex_lock(&(lock->mutex));
    lock->reader_count--;
    // printf("__ reader_count decremented to: %i \n", lock->reader_count);
    if (lock->reader_count == 0) { // release only if it's the last reader:
        pthread_mutex_unlock(&(lock->writer));
    }
    pthread_mutex_unlock(&(lock->mutex));

}

void cleanup(rw_lock* lock)
{
    //TODO: modify as needed
    pthread_mutex_destroy(&(lock->mutex));
    pthread_mutex_destroy(&(lock->writer));
    pthread_mutex_destroy(&(lock->in));
}




/* Takeaways: 
 *
 * 1) when init the mutex using 
     `pthread_mutex_init(&(lock->mutex), NULL);`
      it locks the mutex by default
 * 2) The mutex exists to control the change of shared variables. The roomEmpty mutex exists so that we can control the entry and exit to the rooms (i.e. ensure the writer doesn't enter when the readers are in and the readers don't enter when the writers are in). Realise that the writer count is modified when the writer is in the room (incremented on acquire and decremented on release) so we can let the modificataion of the writer_count variable be synced using the emptyRoom
 * mutex. We still need to use the other mutex for reader_count because multiple readers are allowed in the room...
 *
 * 3) just needed to add an `in` mutex that will indicate interest for a reader/writer  to enter the room. 
 */
