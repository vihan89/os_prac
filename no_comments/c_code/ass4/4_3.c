#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define ITEMS 10

sem_t data_avail;
pthread_mutex_t file_mutex;

void* producer(void* arg) {
    for(int i = 0; i < ITEMS; i++) {
        int data = rand() % 100;
        pthread_mutex_lock(&file_mutex);
        FILE *f = fopen("data.txt", "w");
        fprintf(f, "%d", data);
        fclose(f);
        printf("Produced: %d\n", data);
        pthread_mutex_unlock(&file_mutex);
        sem_post(&data_avail);
        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    for(int i = 0; i < ITEMS; i++) {
        sem_wait(&data_avail);
        pthread_mutex_lock(&file_mutex);
        FILE *f = fopen("data.txt", "r");
        int data;
        fscanf(f, "%d", &data);
        fclose(f);
        printf("Consumed: %d\n", data);
        pthread_mutex_unlock(&file_mutex);
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;
    
    sem_init(&data_avail, 0, 0);
    pthread_mutex_init(&file_mutex, NULL);
    
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    
    sem_destroy(&data_avail);
    pthread_mutex_destroy(&file_mutex);
    
    return 0;
}
