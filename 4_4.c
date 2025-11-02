/*
4.4 Sleeping Barber Problem: 
Suppose a customer arrives and notices that the barber is busy cutting the hair of 
another customer, so he goes to the waiting room. While he is on his way to the waiting 
room, the barber finishes his job and sees the waiting room for other customers. 
But he finds no one in the waiting room (as the customer has yet not arrived in the 
waiting room), so he sits down in his chair and sleeps. Now the barber is waiting for 
new customers to wake him up, and the customer is waiting as he thinks the barber is busy. 
Implement the solution, using three semaphores: one for customers (for counts of waiting 
for customers), one for barber (a binary semaphore denoting the state of the barber, 
i.e., 0 for idle and 1 for busy), and a mutual exclusion semaphore, mutex for seats.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CHAIRS 5
#define NUM_CUSTOMERS 15

int waiting_customers = 0;

sem_t customers;  // Counting semaphore for waiting customers
sem_t barber;     // Binary semaphore for barber state
pthread_mutex_t mutex;  // Mutex for seats

void* barber_thread(void* arg) {
    while(1) {
        printf("Barber: Waiting for customers (sleeping)...\n");
        sem_wait(&customers);  // Wait for customers
        
        pthread_mutex_lock(&mutex);
        waiting_customers--;
        printf("Barber: Customer arrived. Waiting customers: %d\n", waiting_customers);
        pthread_mutex_unlock(&mutex);
        
        sem_post(&barber);  // Barber is ready
        
        printf("Barber: Cutting hair...\n");
        sleep(2);  // Simulate haircut time
        printf("Barber: Finished cutting hair\n\n");
    }
    return NULL;
}

void* customer_thread(void* arg) {
    int customer_id = *(int*)arg;
    
    sleep(rand() % 3);  // Random arrival time
    
    pthread_mutex_lock(&mutex);
    
    if(waiting_customers < NUM_CHAIRS) {
        waiting_customers++;
        printf("Customer %d: Entered shop. Waiting customers: %d\n", customer_id, waiting_customers);
        
        pthread_mutex_unlock(&mutex);
        sem_post(&customers);  // Wake up barber
        sem_wait(&barber);     // Wait for barber to be ready
        
        printf("Customer %d: Getting haircut\n", customer_id);
        
    } else {
        printf("Customer %d: Shop is full. Leaving...\n", customer_id);
        pthread_mutex_unlock(&mutex);
    }
    
    return NULL;
}

int main() {
    pthread_t barber;
    pthread_t customers[NUM_CUSTOMERS];
    int customer_ids[NUM_CUSTOMERS];
    
    // Initialize semaphores and mutex
    sem_init(&customers, 0, 0);
    sem_init(&barber, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    
    printf("=== Sleeping Barber Problem ===\n");
    printf("Number of waiting chairs: %d\n", NUM_CHAIRS);
    printf("Number of customers: %d\n\n", NUM_CUSTOMERS);
    
    // Create barber thread
    pthread_create(&barber, NULL, barber_thread, NULL);
    
    sleep(1);
    
    // Create customer threads
    for(int i = 0; i < NUM_CUSTOMERS; i++) {
        customer_ids[i] = i + 1;
        pthread_create(&customers[i], NULL, customer_thread, &customer_ids[i]);
    }
    
    // Wait for all customers
    for(int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }
    
    printf("\n=== All customers served ===\n");
    
    // Cleanup
    pthread_cancel(barber);
    sem_destroy(&customers);
    sem_destroy(&barber);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
