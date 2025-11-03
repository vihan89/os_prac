

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define NUM_STUDENTS 10
#define NUM_CHAIRS 3

int waiting = 0;
sem_t students;
sem_t ta_available;
pthread_mutex_t mutex;

void* ta_thread(void* arg) {
    cout << "🎓 TA is ready and sleeping...\n" << endl;
    
    while(true) {

        sem_wait(&students);
        
        pthread_mutex_lock(&mutex);
        waiting--;
        cout << "👨‍🏫 TA: Woke up! Students waiting: " << waiting << endl;
        pthread_mutex_unlock(&mutex);
        

        cout << "👨‍🏫 TA: Helping a student..." << endl;
        sleep(2);
        cout << "👨‍🏫 TA: Done helping. Ready for next student.\n" << endl;
        

        sem_post(&ta_available);
    }
    
    return NULL;
}

void* student_thread(void* arg) {
    int student_id = *(int*)arg;
    
    sleep(rand() % 3);
    
    pthread_mutex_lock(&mutex);
    
    if(waiting < NUM_CHAIRS) {

        waiting++;
        cout << "👨‍🎓 Student " << student_id << ": Arrived, waiting (chairs: " 
             << waiting << "/" << NUM_CHAIRS << ")" << endl;
        
        pthread_mutex_unlock(&mutex);
        

        sem_post(&students);
        

        sem_wait(&ta_available);
        
        cout << "👨‍🎓 Student " << student_id << ": Getting help from TA" << endl;
        cout << "✓ Student " << student_id << ": Help received, leaving\n" << endl;
    }
    else {

        cout << "❌ Student " << student_id << ": All chairs full, leaving!\n" << endl;
        pthread_mutex_unlock(&mutex);
    }
    
    return NULL;
}

int main() {
    pthread_t ta;
    pthread_t students[NUM_STUDENTS];
    int student_ids[NUM_STUDENTS];
    
    cout << "========================================" << endl;
    cout << "  SLEEPING TA PROBLEM" << endl;
    cout << "========================================" << endl;
    cout << "Waiting Chairs: " << NUM_CHAIRS << endl;
    cout << "Total Students: " << NUM_STUDENTS << endl;
    cout << "========================================\n" << endl;
    

    sem_init(&students, 0, 0);
    sem_init(&ta_available, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    

    pthread_create(&ta, NULL, ta_thread, NULL);
    

    for(int i = 0; i < NUM_STUDENTS; i++) {
        student_ids[i] = i + 1;
        pthread_create(&students[i], NULL, student_thread, &student_ids[i]);
    }
    

    for(int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(students[i], NULL);
    }
    

    pthread_cancel(ta);
    

    sem_destroy(&students);
    sem_destroy(&ta_available);
    pthread_mutex_destroy(&mutex);
    
    cout << "\n========================================" << endl;
    cout << "  ALL STUDENTS SERVED!" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

