/*
4.5 Readers-Writers Problem (Writers Priority): 
A database must be shared by numerous concurrent processes, some of which may simply 
want to read the database, while others may wish to update (read and write) the database. 
We differentiate between these two processes by referring to the former as Readers and 
the latter as Writers. There will be no negative consequences if two readers access the 
shared data simultaneously. If a writer and another thread (either a reader or a writer) 
access the common data simultaneously, chaos may follow. 
Implement a solution that the writers have exclusive access to the shared database by 
using mutex, and semaphore with readers being prioritized.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

int shared_data = 0;
int reader_count = 0;

pthread_mutex_t mutex;       // Mutex for reader count
pthread_mutex_t db_mutex;    // Mutex for database access

void* reader(void* arg) {
    int reader_id = *(int*)arg;
    
    while(1) {
        sleep(rand() % 3);
        
        // Entry section
        pthread_mutex_lock(&mutex);
        reader_count++;
        if(reader_count == 1) {
            pthread_mutex_lock(&db_mutex);  // First reader locks database
        }
        pthread_mutex_unlock(&mutex);
        
        // Critical section - Reading
        printf("Reader %d: Reading data = %d (Active readers: %d)\n", 
               reader_id, shared_data, reader_count);
        sleep(1);
        
        // Exit section
        pthread_mutex_lock(&mutex);
        reader_count--;
        if(reader_count == 0) {
            pthread_mutex_unlock(&db_mutex);  // Last reader unlocks database
        }
        pthread_mutex_unlock(&mutex);
        
        printf("Reader %d: Finished reading (Active readers: %d)\n", 
               reader_id, reader_count);
        
        sleep(2);
    }
    
    return NULL;
}

void* writer(void* arg) {
    int writer_id = *(int*)arg;
    
    while(1) {
        sleep(rand() % 4);
        
        // Entry section
        pthread_mutex_lock(&db_mutex);
        
        // Critical section - Writing
        shared_data++;
        printf("Writer %d: Writing data = %d\n", writer_id, shared_data);
        sleep(2);
        printf("Writer %d: Finished writing\n", writer_id);
        
        // Exit section
        pthread_mutex_unlock(&db_mutex);
        
        sleep(3);
    }
    
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];
    
    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&db_mutex, NULL);
    
    printf("=== Readers-Writers Problem (Writers Priority) ===\n");
    printf("Number of readers: %d\n", NUM_READERS);
    printf("Number of writers: %d\n\n", NUM_WRITERS);
    
    // Create reader threads
    for(int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }
    
    // Create writer threads
    for(int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }
    
    // Run for some time
    sleep(30);
    
    printf("\n=== Stopping simulation ===\n");
    
    // Cleanup
    for(int i = 0; i < NUM_READERS; i++) {
        pthread_cancel(readers[i]);
    }
    for(int i = 0; i < NUM_WRITERS; i++) {
        pthread_cancel(writers[i]);
    }
    
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&db_mutex);
    
    return 0;
}
