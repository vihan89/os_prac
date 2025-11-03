

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define BUFFER_SIZE 5
#define NUM_ITEMS 10

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void* producer(void* arg) {
    for(int i = 1; i <= NUM_ITEMS; i++) {

        sem_wait(&empty);
        

        pthread_mutex_lock(&mutex);
        

        buffer[in] = i;
        cout << "🍕 Simpson bakes pizza " << i << " (placed at slot " << in << ")" << endl;
        in = (in + 1) % BUFFER_SIZE;

        

        pthread_mutex_unlock(&mutex);
        

        sem_post(&full);
        
        sleep(1);
    }
    
    cout << "\n✓ Simpson finished baking all pizzas!" << endl;
    return NULL;
}

void* consumer(void* arg) {
    for(int i = 1; i <= NUM_ITEMS; i++) {

        sem_wait(&full);
        

        pthread_mutex_lock(&mutex);
        

        int pizza = buffer[out];
        cout << "🍴 Joey eats pizza " << pizza << " (from slot " << out << ")" << endl;
        out = (out + 1) % BUFFER_SIZE;

        

        pthread_mutex_unlock(&mutex);
        

        sem_post(&empty);
        
        sleep(2);
    }
    
    cout << "\n✓ Joey finished eating all pizzas!" << endl;
    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread;
    
    cout << "========================================" << endl;
    cout << "  PRODUCER-CONSUMER PROBLEM" << endl;
    cout << "  (Simpson Bakes, Joey Eats)" << endl;
    cout << "========================================" << endl;
    cout << "Buffer Size: " << BUFFER_SIZE << " pizzas" << endl;
    cout << "Total Pizzas: " << NUM_ITEMS << endl;
    cout << "========================================\n" << endl;
    

    

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    

    pthread_mutex_init(&mutex, NULL);
    

    

    pthread_create(&prod_thread, NULL, producer, NULL);
    

    pthread_create(&cons_thread, NULL, consumer, NULL);
    

    
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);
    

    
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    
    cout << "\n========================================" << endl;
    cout << "  ALL OPERATIONS COMPLETED!" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

