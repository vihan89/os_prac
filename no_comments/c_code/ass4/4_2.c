#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define STUDENTS 10
#define CHAIRS 3

int waiting = 0, students_helped = 0;
sem_t students_sem, ta_sem;
pthread_mutex_t mutex;

void* ta_thread(void* arg) {
    while(students_helped < STUDENTS) {
        sem_wait(&students_sem);
        pthread_mutex_lock(&mutex);
        waiting--;
        students_helped++;
        printf("TA helping student (waiting: %d)\n", waiting);
        pthread_mutex_unlock(&mutex);
        sleep(2);
        sem_post(&ta_sem);
    }
    printf("TA: All students helped. Going home.\n");
    return NULL;
}

void* student_thread(void* arg) {
    int id = *(int*)arg;
    sleep(rand() % 3);
    
    pthread_mutex_lock(&mutex);
    if(waiting < CHAIRS) {
        waiting++;
        printf("Student %d waiting (%d/%d)\n", id, waiting, CHAIRS);
        pthread_mutex_unlock(&mutex);
        sem_post(&students_sem);
        sem_wait(&ta_sem);
        printf("Student %d got help\n", id);
    } else {
        printf("Student %d left (chairs full)\n", id);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t ta, students[STUDENTS];
    int ids[STUDENTS];
    
    sem_init(&students_sem, 0, 0);
    sem_init(&ta_sem, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    
    pthread_create(&ta, NULL, ta_thread, NULL);
    
    for(int i = 0; i < STUDENTS; i++) {
        ids[i] = i + 1;
        pthread_create(&students[i], NULL, student_thread, &ids[i]);
    }
    
    for(int i = 0; i < STUDENTS; i++)
        pthread_join(students[i], NULL);
    
    pthread_join(ta, NULL);
    sem_destroy(&students_sem);
    sem_destroy(&ta_sem);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
