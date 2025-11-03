/*
4.6 Readers-Writers Problem (Readers Priority): 
A real-world example of the readers-writers problem is an airline reservation system 
where Readers want to read flight information and Writers want to make flight reservations. 
Potential problem: if readers and writers can access the shared data simultaneously then 
readers/writers may view flights as being available when they've actually just been booked. 
Implement the solution using mutex and semaphore that controls access to the reader count 
and database given the priority to readers over writers.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

int available_seats = 100;
int reader_count = 0;

sem_t db_sem;           // Semaphore for database access
pthread_mutex_t mutex;  // Mutex for reader count

void* reader(void* arg) {
    int reader_id = *(int*)arg;
    
    while(1) {
        sleep(rand() % 2);
        
        // Entry section
        pthread_mutex_lock(&mutex);
        reader_count++;
        if(reader_count == 1) {
            sem_wait(&db_sem);  // First reader locks database
        }
        pthread_mutex_unlock(&mutex);
        
        // Critical section - Reading
        printf("Reader %d: Checking flight info - Available seats: %d (Active readers: %d)\n", 
               reader_id, available_seats, reader_count);
        sleep(1);
        
        // Exit section
        pthread_mutex_lock(&mutex);
        reader_count--;
        printf("Reader %d: Finished reading (Active readers: %d)\n", 
               reader_id, reader_count);
        if(reader_count == 0) {
            sem_post(&db_sem);  // Last reader unlocks database
        }
        pthread_mutex_unlock(&mutex);
        
        sleep(2);
    }
    
    return NULL;
}

void* writer(void* arg) {
    int writer_id = *(int*)arg;
    
    while(1) {
        sleep(rand() % 4);
        
        // Entry section
        sem_wait(&db_sem);
        
        // Critical section - Writing (booking seat)
        if(available_seats > 0) {
            available_seats--;
            printf("Writer %d: Booked 1 seat. Remaining seats: %d\n", 
                   writer_id, available_seats);
        } else {
            printf("Writer %d: No seats available!\n", writer_id);
        }
        sleep(2);
        printf("Writer %d: Finished booking\n", writer_id);
        
        // Exit section
        sem_post(&db_sem);
        
        sleep(3);
    }
    
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];
    
    // Initialize semaphore and mutex
    sem_init(&db_sem, 0, 1);
    pthread_mutex_init(&mutex, NULL);
    
    printf("=== Airline Reservation System (Readers Priority) ===\n");
    printf("Initial available seats: %d\n", available_seats);
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
    printf("Final available seats: %d\n", available_seats);
    
    // Cleanup
    for(int i = 0; i < NUM_READERS; i++) {
        pthread_cancel(readers[i]);
    }
    for(int i = 0; i < NUM_WRITERS; i++) {
        pthread_cancel(writers[i]);
    }
    
    sem_destroy(&db_sem);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
