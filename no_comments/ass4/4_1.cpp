#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

#define SIZE 5
#define ITEMS 10

int buffer[SIZE], in = 0, out = 0;
sem_t sem_empty, sem_full;
pthread_mutex_t mutex;

void* producer(void* arg) {
    for(int i = 1; i <= ITEMS; i++) {
        sem_wait(&sem_empty);
        pthread_mutex_lock(&mutex);
        
        buffer[in] = i;
        cout << "Produced: " << i << " at " << in << endl;
        in = (in + 1) % SIZE;
        
        pthread_mutex_unlock(&mutex);
        sem_post(&sem_full);
        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    for(int i = 1; i <= ITEMS; i++) {
        sem_wait(&sem_full);
        pthread_mutex_lock(&mutex);
        
        int item = buffer[out];
        cout << "Consumed: " << item << " from " << out << endl;
        out = (out + 1) % SIZE;
        
        pthread_mutex_unlock(&mutex);
        sem_post(&sem_empty);
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;
    
    sem_init(&sem_empty, 0, SIZE);
    sem_init(&sem_full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    
    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}

