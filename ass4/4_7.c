/*
4.7 Airline Reservation System: 
Consider an airline reservation system in which many clients are attempting to book seats 
on particular flights between particular cities. All of the information about flights and 
seats is stored in a common database in memory. A seat that was available during probing 
phase could easily be booked by someone else before the client has a chance to book it. 
The client probing around the database is called a reader. The client attempting to book 
the flight is called a writer. 
Implement a C program that launches multiple reader threads and multiple writer threads, 
each attempting to access a single reservation record. Implement a version of your program 
that uses mutex and counting semaphore to enforce a disciplined protocol for readers and 
writers accessing the shared reservation data. In particular, your program should favor 
readers to access the shared data simultaneously over writers.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 8
#define NUM_WRITERS 3
#define TOTAL_SEATS 50

typedef struct {
    int flight_id;
    int available_seats;
} FlightRecord;

FlightRecord flight = {101, TOTAL_SEATS};
int reader_count = 0;

sem_t resource_sem;     // Counting semaphore for resource access
pthread_mutex_t mutex;  // Mutex for reader count

void* reader_thread(void* arg) {
    int reader_id = *(int*)arg;
    
    for(int i = 0; i < 3; i++) {
        sleep(rand() % 2);
        
        // Entry section
        pthread_mutex_lock(&mutex);
        reader_count++;
        if(reader_count == 1) {
            sem_wait(&resource_sem);  // First reader locks resource
        }
        pthread_mutex_unlock(&mutex);
        
        // Critical section - Reading (Probing)
        printf("Reader %d: Probing Flight %d - Available seats: %d (Active readers: %d)\n", 
               reader_id, flight.flight_id, flight.available_seats, reader_count);
        sleep(1);
        
        // Exit section
        pthread_mutex_lock(&mutex);
        reader_count--;
        printf("Reader %d: Finished probing (Active readers: %d)\n", 
               reader_id, reader_count);
        if(reader_count == 0) {
            sem_post(&resource_sem);  // Last reader unlocks resource
        }
        pthread_mutex_unlock(&mutex);
        
        sleep(1);
    }
    
    return NULL;
}

void* writer_thread(void* arg) {
    int writer_id = *(int*)arg;
    
    for(int i = 0; i < 2; i++) {
        sleep(rand() % 3);
        
        // Entry section
        printf("Writer %d: Waiting to book...\n", writer_id);
        sem_wait(&resource_sem);
        
        // Critical section - Writing (Booking)
        if(flight.available_seats > 0) {
            int seats_to_book = (rand() % 3) + 1;  // Book 1-3 seats
            if(seats_to_book > flight.available_seats) {
                seats_to_book = flight.available_seats;
            }
            flight.available_seats -= seats_to_book;
            printf("Writer %d: Booked %d seat(s). Remaining: %d\n", 
                   writer_id, seats_to_book, flight.available_seats);
        } else {
            printf("Writer %d: Flight is full! Cannot book.\n", writer_id);
        }
        sleep(2);
        
        // Exit section
        sem_post(&resource_sem);
        printf("Writer %d: Finished booking\n", writer_id);
        
        sleep(2);
    }
    
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];
    
    srand(time(NULL));
    
    // Initialize semaphore and mutex
    sem_init(&resource_sem, 0, 1);
    pthread_mutex_init(&mutex, NULL);
    
    printf("=== Airline Reservation System ===\n");
    printf("Flight ID: %d\n", flight.flight_id);
    printf("Initial available seats: %d\n", flight.available_seats);
    printf("Number of readers: %d\n", NUM_READERS);
    printf("Number of writers: %d\n\n", NUM_WRITERS);
    
    // Create reader threads
    for(int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader_thread, &reader_ids[i]);
    }
    
    // Create writer threads
    for(int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer_thread, &writer_ids[i]);
    }
    
    // Wait for all threads
    for(int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for(int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }
    
    printf("\n=== Reservation Complete ===\n");
    printf("Final available seats: %d\n", flight.available_seats);
    printf("Seats booked: %d\n", TOTAL_SEATS - flight.available_seats);
    
    // Cleanup
    sem_destroy(&resource_sem);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
