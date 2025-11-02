/*
4.2 Sleeping TA Problem: 
A university computer science department has a teaching assistant (TA) who helps 
undergraduate students with their programming assignments during regular office hours. 
The TA's office is rather small and has room for only one desk with a chair and computer. 
There are three chairs in the hallway outside the office where students can sit and wait 
if the TA is currently helping another student. When there are no students who need help 
during office hours, the TA sits at the desk and takes a nap. If a student arrives during 
office hours and finds the TA sleeping, the student must awaken the TA to ask for help. 
If a student arrives and finds the TA currently helping another student, the student sits 
on one of the chairs in the hallway and waits. If no chairs are available, the student 
will come back at a later time. 
Using POSIX threads, mutex locks, and semaphores, implement a solution that coordinates 
the activities of the TA and the students.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CHAIRS 3
#define NUM_STUDENTS 10

int waiting_students = 0;
int ta_sleeping = 1;

sem_t students_sem;  // Semaphore for waiting students
sem_t ta_sem;        // Semaphore for TA
pthread_mutex_t mutex;

void* ta_thread(void* arg) {
    while(1) {
        sem_wait(&students_sem);  // Wait for students
        pthread_mutex_lock(&mutex);
        
        waiting_students--;
        ta_sleeping = 0;
        
        pthread_mutex_unlock(&mutex);
        sem_post(&ta_sem);  // Signal student that TA is ready
        
        printf("TA: Helping a student...\n");
        sleep(2);  // Simulate helping time
        printf("TA: Finished helping student\n");
        
        pthread_mutex_lock(&mutex);
        if(waiting_students == 0) {
            ta_sleeping = 1;
            printf("TA: No more students. Going to sleep...\n");
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* student_thread(void* arg) {
    int student_id = *(int*)arg;
    
    sleep(rand() % 5);  // Random arrival time
    
    pthread_mutex_lock(&mutex);
    
    if(waiting_students < NUM_CHAIRS) {
        waiting_students++;
        printf("Student %d: Arrived. Waiting students: %d\n", student_id, waiting_students);
        
        if(ta_sleeping) {
            printf("Student %d: Waking up TA\n", student_id);
        }
        
        pthread_mutex_unlock(&mutex);
        sem_post(&students_sem);  // Wake up TA
        sem_wait(&ta_sem);        // Wait for TA to be ready
        
        printf("Student %d: Getting help from TA\n", student_id);
        
    } else {
        printf("Student %d: No chairs available. Will come back later.\n", student_id);
        pthread_mutex_unlock(&mutex);
    }
    
    return NULL;
}

int main() {
    pthread_t ta;
    pthread_t students[NUM_STUDENTS];
    int student_ids[NUM_STUDENTS];
    
    // Initialize semaphores and mutex
    sem_init(&students_sem, 0, 0);
    sem_init(&ta_sem, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    
    printf("=== Sleeping TA Problem Simulation ===\n");
    printf("Number of chairs: %d\n", NUM_CHAIRS);
    printf("Number of students: %d\n\n", NUM_STUDENTS);
    
    // Create TA thread
    pthread_create(&ta, NULL, ta_thread, NULL);
    printf("TA: Going to sleep (no students)...\n\n");
    
    // Create student threads
    for(int i = 0; i < NUM_STUDENTS; i++) {
        student_ids[i] = i + 1;
        pthread_create(&students[i], NULL, student_thread, &student_ids[i]);
    }
    
    // Wait for all students to finish
    for(int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(students[i], NULL);
    }
    
    printf("\n=== All students served ===\n");
    
    // Cleanup
    pthread_cancel(ta);
    sem_destroy(&students_sem);
    sem_destroy(&ta_sem);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
