/*
4.1 Producer-Consumer Problem: 
The producer is Mr. Simpson whose job is to bake Pizza and consumer is Joey Tribbiani 
who consumes Pizza at the same time. Both of them share common Pizza counter to interact 
with each other. The problem is to make sure that the Simpson won't try to add more pizza 
on Pizza-counter if it's full. He needs to wait until Joey consumes Pizza. 
Similarly, Joey can't consume pizza from an empty counter. He needs to wait until 
Mr. Simpson adds Pizza on counter. 
Implement a solution in C using POSIX threads that coordinates the activities of the 
producer Simpson and the consumer Joey with counting semaphores and mutex.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PIZZAS 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty;  // Count of empty slots
sem_t full;   // Count of full slots
pthread_mutex_t mutex;

void* simpson(void* arg) {
    for(int i = 1; i <= NUM_PIZZAS; i++) {
        sem_wait(&empty);  // Wait for empty slot
        pthread_mutex_lock(&mutex);
        
        // Critical section - Add pizza
        buffer[in] = i;
        printf("Mr. Simpson baked Pizza %d at position %d\n", i, in);
        in = (in + 1) % BUFFER_SIZE;
        
        pthread_mutex_unlock(&mutex);
        sem_post(&full);  // Signal that slot is full
        
        sleep(1);  // Simulate baking time
    }
    printf("\nMr. Simpson: All pizzas baked!\n");
    return NULL;
}

void* joey(void* arg) {
    for(int i = 1; i <= NUM_PIZZAS; i++) {
        sem_wait(&full);  // Wait for full slot
        pthread_mutex_lock(&mutex);
        
        // Critical section - Consume pizza
        int pizza = buffer[out];
        printf("Joey consumed Pizza %d from position %d\n", pizza, out);
        out = (out + 1) % BUFFER_SIZE;
        
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);  // Signal that slot is empty
        
        sleep(2);  // Simulate eating time
    }
    printf("\nJoey: All pizzas consumed!\n");
    return NULL;
}

int main() {
    pthread_t producer, consumer;
    
    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);  // Initially all slots are empty
    sem_init(&full, 0, 0);              // Initially no slots are full
    pthread_mutex_init(&mutex, NULL);
    
    printf("=== Pizza Counter Simulation ===\n");
    printf("Buffer Size: %d\n", BUFFER_SIZE);
    printf("Total Pizzas: %d\n\n", NUM_PIZZAS);
    
    // Create threads
    pthread_create(&producer, NULL, simpson, NULL);
    pthread_create(&consumer, NULL, joey, NULL);
    
    // Wait for threads to finish
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    
    // Cleanup
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    
    printf("\n=== Simulation Complete ===\n");
    
    return 0;
}
