// Sleeping TA Problem
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

int wait = 0;
sem_t s, t;
pthread_mutex_t m;

void* ta(void* arg) {
    for(int i = 0; i < 10; i++) {
        sem_wait(&s);
        pthread_mutex_lock(&m);
        wait--;
        cout << "TA help (wait=" << wait << ")\n";
        pthread_mutex_unlock(&m);
        sem_post(&t);
    }
    return NULL;
}

void* student(void* arg) {
    int id = *(int*)arg;
    pthread_mutex_lock(&m);
    if(wait < 3) {
        wait++;
        cout << "S" << id << " sit (" << wait << "/3)\n";
        pthread_mutex_unlock(&m);
        sem_post(&s);
        sem_wait(&t);
        cout << "S" << id << " done\n";
    } else {
        cout << "S" << id << " left\n";
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main() {
    pthread_t ta_t, st[10];
    int id[10];
    
    sem_init(&s, 0, 0);
    sem_init(&t, 0, 0);
    pthread_mutex_init(&m, NULL);
    
    pthread_create(&ta_t, NULL, ta, NULL);
    for(int i = 0; i < 10; i++) {
        id[i] = i+1;
        pthread_create(&st[i], NULL, student, &id[i]);
    }
    
    for(int i = 0; i < 10; i++) pthread_join(st[i], NULL);
    pthread_join(ta_t, NULL);
    
    sem_destroy(&s);
    sem_destroy(&t);
    pthread_mutex_destroy(&m);
    return 0;
}