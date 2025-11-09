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
ew    int id[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    sem_init(&s, 0, 0);
    sem_init(&t, 0, 0);
    pthread_mutex_init(&m, NULL);
    
    pthread_create(&ta_t, NULL, ta, NULL);
    pthread_create(&st[0], NULL, student, &id[0]);
    pthread_create(&st[1], NULL, student, &id[1]);
    pthread_create(&st[2], NULL, student, &id[2]);
    pthread_create(&st[3], NULL, student, &id[3]);
    pthread_create(&st[4], NULL, student, &id[4]);
    pthread_create(&st[5], NULL, student, &id[5]);
    pthread_create(&st[6], NULL, student, &id[6]);
    pthread_create(&st[7], NULL, student, &id[7]);
    pthread_create(&st[8], NULL, student, &id[8]);
    pthread_create(&st[9], NULL, student, &id[9]);
    
    pthread_join(st[0], NULL);
    pthread_join(st[1], NULL);
    pthread_join(st[2], NULL);
    pthread_join(st[3], NULL);
    pthread_join(st[4], NULL);
    pthread_join(st[5], NULL);
    pthread_join(st[6], NULL);
    pthread_join(st[7], NULL);
    pthread_join(st[8], NULL);
    pthread_join(st[9], NULL);
    pthread_join(ta_t, NULL);
    
    sem_destroy(&s);
    sem_destroy(&t);
    pthread_mutex_destroy(&m);
    return 0;
}