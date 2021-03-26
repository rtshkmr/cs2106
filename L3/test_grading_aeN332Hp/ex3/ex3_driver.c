/*************************************
 * Lab 3 Exercise 2
 *************************************
You may change this file during your own testing, 
but note that they will be replaced with the original 
files when we test your assignments.
 *************************************/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rw_lock.h"

int READERS;
int WRITERS;
int WRITE_COUNT;
int READ_COUNT;

int value = 0;

int writer_read = 0;
int max_concurrent_readers = 0;
pthread_mutex_t max_mutex = PTHREAD_MUTEX_INITIALIZER;

rw_lock* read_write_lock;

void* writer(void* threadid);
void* reader(void* threadid);

int main(int argc, char** argv)
{
    if (argc < 5) {
        printf("Usage: %s readers writers read_count write_count\n", argv[0]);
        exit(1);
    }

    READERS = atoi(argv[1]);
    WRITERS = atoi(argv[2]);
    READ_COUNT = atoi(argv[3]);
    WRITE_COUNT = atoi(argv[4]);

    pthread_t writer_threads[WRITERS];
    pthread_t reader_threads[READERS];

    read_write_lock = malloc(sizeof(rw_lock));
    if (read_write_lock == NULL) {
        printf("Lock failed to be allocated.\n");
        exit(1);
    }

    initialise(read_write_lock);

    int thread_id;
    int i, return_code;
    int bad_threads = 0;
    void* thread_return;
    int *pi;


    for (i = 0; i < READERS; i++) {
        thread_id = i + 1;
        pi = malloc(sizeof(int));
        if (pi == NULL) {
            printf("Pointer allocation failed.\n");
            exit(1);
        }
        *pi = thread_id;
#ifdef DEBUG
        printf("Creating thread for reader #%ld\n", thread_id);
#endif
        return_code = pthread_create(&reader_threads[i], NULL, reader, (void*)pi);
        if (return_code) {
            printf("Error while creating thread. Return code: %d\n", return_code);
            exit(1);
        }
    }
    
    for (i = 0; i < WRITERS; i++) {
        thread_id = i + 1;
        pi = malloc(sizeof(int));
        if (pi == NULL) {
            printf("Pointer allocation failed.\n");
            exit(1);
        }
        *pi = thread_id;
#ifdef DEBUG
        printf("Creating thread for writer #%ld\n", thread_id);
#endif
        return_code = pthread_create(&writer_threads[i], NULL, writer, (void*)pi);
        if (return_code) {
            printf("Error while creating thread. Return code: %d\n", return_code);
            exit(1);
        }
  
    }

    

    for (i = 0; i < WRITERS; i++) {
        pthread_join(writer_threads[i], &thread_return);
        bad_threads += *(int*)thread_return;
        free (thread_return);
    }

    for (i = 0; i < READERS; i++) {
        pthread_join(reader_threads[i], &thread_return);
        bad_threads += *(int*)thread_return;
        free (thread_return);
        
    }

    pthread_mutex_destroy(&max_mutex);

    cleanup(read_write_lock);
    free(read_write_lock);
    if (bad_threads) {
        printf("Program failed: %d bad threads found.\n", bad_threads);
        return -1;
    }
    if (!writer_read) {
        printf("Program failed: All writing operations happen after reading.\n");
        return -1;
    }
    printf("SUCCESS!\n");
    printf("Total writes: %d, Total reads: %d, Max Concurrent Readers: %d\n",
           WRITERS * WRITE_COUNT,
           READERS * READ_COUNT,
           max_concurrent_readers);
    return 0;
}

// Each writer thread runs this function.
// It writes its `threadid` to `value` `WRITE_COUNT` times.
void* writer(void* threadid)
{
    int i;
    int error_found = 0;
    int tid = *(int*)threadid;
    int *pi;
    free (threadid);

    for (i = 0; i < WRITE_COUNT; i++) {

        writer_acquire(read_write_lock);

        if (read_write_lock->reader_count != 0 || read_write_lock->writer_count != 1) {
            printf("Writer %d: Reader or another writer found while attempting to write\n",
                    tid);
            error_found = 1;
        }
#ifdef DEBUG
        printf("[Writer: #%d, Loop: %d] ", tid, i);
#endif
        printf("Writer %d writes.\n", tid);
        value = tid;
        writer_release(read_write_lock);
    }
    pi = malloc(sizeof(int));
    if (pi == NULL) {
        printf("Pointer allocation failed.\n");
        exit(1);
    }
    *pi = error_found;
    pthread_exit((void*)pi);
}

// Each reader thread runs this function.
// It reads and `value` `READ_COUNT` times, and finds the number of
// concurrent readers.
void* reader(void* threadid)
{
    int i, curr_readers;
    int error_found = 0;
    int tid = *(int*)threadid;
    int *pi;
    free (threadid);
    
    for (i = 0; i < READ_COUNT; i++) {

        reader_acquire(read_write_lock);

        if (read_write_lock->writer_count != 0) {
            printf("Writer found while attempting to read\n");
            error_found = 1;
        }
        curr_readers = read_write_lock->reader_count;
        pthread_mutex_lock(&max_mutex);
        if (curr_readers > max_concurrent_readers) {
            max_concurrent_readers = curr_readers;
        }
        pthread_mutex_unlock(&max_mutex);
#ifdef DEBUG
        printf("[Reader: #%d, Loop: %d] ", tid, i);
#endif
        printf("Reader %d reads: %d\n", tid, value);

        // NOTE THE CHANGES:
        usleep(100);
        writer_read = value;
        reader_release(read_write_lock);
        
    }
    
    pi = malloc(sizeof(int));
    if (pi == NULL) {
       printf("Pointer allocation failed.\n");
       exit(1);
    }
    *pi = error_found;
    pthread_exit((void*)pi);

}
