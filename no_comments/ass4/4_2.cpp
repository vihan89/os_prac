#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

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
        cout << "TA helping student (waiting: " << waiting << ")" << endl;
        pthread_mutex_unlock(&mutex);
        sleep(2);
        sem_post(&ta_sem);
    }
    cout << "TA: All students helped. Going home." << endl;
    return NULL;
}

void* student_thread(void* arg) {
    int id = *(int*)arg;
    sleep(rand() % 3);
    
    pthread_mutex_lock(&mutex);
    if(waiting < CHAIRS) {
        waiting++;
        cout << "Student " << id << " waiting (" << waiting << "/" << CHAIRS << ")" << endl;
        pthread_mutex_unlock(&mutex);
        sem_post(&students_sem);
        sem_wait(&ta_sem);
        cout << "Student " << id << " got help" << endl;
    } else {
        cout << "Student " << id << " left (chairs full)" << endl;
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