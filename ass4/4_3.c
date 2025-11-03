/*
4.3 Producer-Consumer with File I/O: 
The Producer generates an integer between 0 and 9 (inclusive), stores it in a file. 
To make the synchronization problem more interesting, the Producer sleeps for a random 
amount of time between 0 and 100 milliseconds before repeating the number-generating cycle. 
The Consumer consumes all integers from the same file as quickly as they become available. 
The activities of the Producer and the Consumer must be synchronized in a following way: 
The two threads must do some simple coordination. That is, the Producer must have a way 
to indicate to the Consumer that the value is ready, and the Consumer must have a way to 
indicate that the value has been retrieved. 
Implement the solution using mutex, and binary semaphore.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define FILENAME "shared_data.txt"
#define NUM_ITEMS 15

sem_t empty;  // Binary semaphore - file is empty
sem_t full;   // Binary semaphore - file has data
pthread_mutex_t file_mutex;

void* producer(void* arg) {
    FILE *fp;
    
    for(int i = 0; i < NUM_ITEMS; i++) {
        sem_wait(&empty);  // Wait until consumer has read previous value
        pthread_mutex_lock(&file_mutex);
        
        // Generate random number between 0 and 9
        int num = rand() % 10;
        
        // Write to file
        fp = fopen(FILENAME, "w");
        if(fp != NULL) {
            fprintf(fp, "%d", num);
            fclose(fp);
            printf("Producer: Generated and wrote %d to file\n", num);
        }
        
        pthread_mutex_unlock(&file_mutex);
        sem_post(&full);  // Signal that value is ready
        
        // Sleep for random time (0-100 ms)
        usleep((rand() % 100) * 1000);
    }
    
    printf("\nProducer: Finished producing all items\n");
    return NULL;
}

void* consumer(void* arg) {
    FILE *fp;
    
    for(int i = 0; i < NUM_ITEMS; i++) {
        sem_wait(&full);  // Wait until producer has written value
        pthread_mutex_lock(&file_mutex);
        
        // Read from file
        fp = fopen(FILENAME, "r");
        if(fp != NULL) {
            int num;
            fscanf(fp, "%d", &num);
            fclose(fp);
            printf("Consumer: Read %d from file\n", num);
        }
        
        pthread_mutex_unlock(&file_mutex);
        sem_post(&empty);  // Signal that value has been consumed
        
        usleep(50000);  // Small delay
    }
    
    printf("\nConsumer: Finished consuming all items\n");
    return NULL;
}

int main() {
    pthread_t prod, cons;
    
    srand(time(NULL));
    
    // Initialize semaphores and mutex
    sem_init(&empty, 0, 1);  // Initially file is empty
    sem_init(&full, 0, 0);   // Initially no data available
    pthread_mutex_init(&file_mutex, NULL);
    
    printf("=== Producer-Consumer with File I/O ===\n");
    printf("Filename: %s\n", FILENAME);
    printf("Items to produce/consume: %d\n\n", NUM_ITEMS);
    
    // Create threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    
    // Wait for threads to finish
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    
    // Cleanup
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&file_mutex);
    remove(FILENAME);  // Delete the file
    
    printf("\n=== Simulation Complete ===\n");
    
    return 0;
}
