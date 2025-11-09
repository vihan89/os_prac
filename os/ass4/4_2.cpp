/*
==============================================================================
ASSIGNMENT 4.2: SLEEPING TA PROBLEM
==============================================================================

PROBLEM:
- TA (Teaching Assistant) sleeps when no students present
- Students arrive and wait in chairs if TA busy
- 3 waiting chairs + 1 TA chair (total 4 students max)
- Students leave if all chairs full
- TA wakes up when student arrives
- Use semaphores and mutex

COMPILATION:
g++ 4_2.cpp -o 4_2 -lpthread

EXECUTION:
./4_2

KEY CONCEPTS:
1. Finite waiting room (3 chairs)
2. TA sleeps/wakes based on students
3. Students wait in queue
4. FIFO service order
5. Students leave if room full

SEMAPHORES:
- students: Counts waiting students (wakes TA)
- ta_available: TA availability (0=busy, 1=free)
- mutex: Protects waiting count

SAMPLE RUN:
Student 1 arrives, wakes TA
TA helping student 1
Student 2 arrives, sits in waiting room
...
==============================================================================
*/

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define NUM_STUDENTS 10
#define NUM_CHAIRS 3

// ============================================================================
// SHARED VARIABLES
// ============================================================================
int waiting = 0;                // Number of students waiting
sem_t students;                 // Counts students (wakes TA)
sem_t ta_available;             // TA availability
pthread_mutex_t mutex;          // Protects waiting counter

// ============================================================================
// TA THREAD
// ============================================================================
void* ta_thread(void* arg) {
    cout << "🎓 TA is ready and sleeping...\n" << endl;
    
    while(true) {
        // Wait for student to arrive
        sem_wait(&students);            // Sleep until student arrives
        
        pthread_mutex_lock(&mutex);
        waiting--;                      // One less student waiting
        cout << "👨‍🏫 TA: Woke up! Students waiting: " << waiting << endl;
        pthread_mutex_unlock(&mutex);
        
        // Help student (critical section)
        cout << "👨‍🏫 TA: Helping a student..." << endl;
        sleep(2);                       // Simulate helping time
        cout << "👨‍🏫 TA: Done helping. Ready for next student.\n" << endl;
        
        // Signal TA is available
        sem_post(&ta_available);
    }
    
    return NULL;
}

// ============================================================================
// STUDENT THREAD
// ============================================================================
void* student_thread(void* arg) {
    int student_id = *(int*)arg;
    
    sleep(rand() % 3);              // Random arrival time
    
    pthread_mutex_lock(&mutex);
    
    if(waiting < NUM_CHAIRS) {
        // Chairs available, sit and wait
        waiting++;
        cout << "👨‍🎓 Student " << student_id << ": Arrived, waiting (chairs: " 
             << waiting << "/" << NUM_CHAIRS << ")" << endl;
        
        pthread_mutex_unlock(&mutex);
        
        // Wake up TA
        sem_post(&students);
        
        // Wait for TA to be available
        sem_wait(&ta_available);
        
        cout << "👨‍🎓 Student " << student_id << ": Getting help from TA" << endl;
        cout << "✓ Student " << student_id << ": Help received, leaving\n" << endl;
    }
    else {
        // All chairs full, leave
        cout << "❌ Student " << student_id << ": All chairs full, leaving!\n" << endl;
        pthread_mutex_unlock(&mutex);
    }
    
    return NULL;
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================
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
    
    // ========================================================================
    // INITIALIZE SYNCHRONIZATION
    // ========================================================================
    sem_init(&students, 0, 0);          // Initially no students
    sem_init(&ta_available, 0, 0);      // TA not available initially
    pthread_mutex_init(&mutex, NULL);
    
    // ========================================================================
    // CREATE TA THREAD
    // ========================================================================
    pthread_create(&ta, NULL, ta_thread, NULL);
    
    // ========================================================================
    // CREATE STUDENT THREADS
    // ========================================================================
    for(int i = 0; i < NUM_STUDENTS; i++) {
        student_ids[i] = i + 1;
        pthread_create(&students[i], NULL, student_thread, &student_ids[i]);
    }
    
    // ========================================================================
    // WAIT FOR STUDENTS TO COMPLETE
    // ========================================================================
    for(int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(students[i], NULL);
    }
    
    // Cancel TA thread (it runs forever)
    pthread_cancel(ta);
    
    // ========================================================================
    // CLEANUP
    // ========================================================================
    sem_destroy(&students);
    sem_destroy(&ta_available);
    pthread_mutex_destroy(&mutex);
    
    cout << "\n========================================" << endl;
    cout << "  ALL STUDENTS SERVED!" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

/*
==============================================================================
IMPORTANT NOTES FOR EXAM:

1. SLEEPING TA PROBLEM OVERVIEW:
   - TA office has one desk and N chairs
   - If no students, TA sleeps
   - When student arrives:
     * If TA sleeping, wake TA and get help
     * If TA busy and chairs available, sit and wait
     * If all chairs full, leave
   - TA helps students one at a time in FIFO order

2. SYNCHRONIZATION REQUIREMENTS:
   - Mutual exclusion: Only one student with TA
   - Counting: Track number of waiting students
   - Signaling: Wake TA when student arrives
   - Bounded waiting: Limited chairs

3. SOLUTION COMPONENTS:
   
   Semaphore 'students':
   - Counts number of waiting students
   - Used by students to wake TA
   - TA waits on this semaphore
   
   Semaphore 'ta_available':
   - Binary semaphore (0 or 1)
   - Signals TA is ready to help
   - Students wait on this
   
   Mutex:
   - Protects 'waiting' counter
   - Ensures atomic check of chair availability

4. TA LOGIC:
   while (true) {
       wait(students);         // Sleep until student arrives
       lock(mutex);
       waiting--;              // Reduce waiting count
       unlock(mutex);
       help_student();         // Help the student
       signal(ta_available);   // Ready for next
   }

5. STUDENT LOGIC:
   lock(mutex);
   if (waiting < NUM_CHAIRS) {
       waiting++;
       unlock(mutex);
       signal(students);       // Wake TA
       wait(ta_available);     // Wait for help
       get_help();
   } else {
       unlock(mutex);
       leave();                // No chairs, leave
   }

6. KEY DIFFERENCES FROM PRODUCER-CONSUMER:
   - Bounded waiting room (not circular buffer)
   - Service provider sleeps when idle
   - Clients can be rejected (full waiting room)
   - FIFO service order

7. RACE CONDITIONS PREVENTED:
   - Multiple students checking chairs simultaneously
   - TA and student modifying waiting count together
   - Student arriving while TA checking queue

8. EXAM SCENARIOS:
   
   Scenario 1: TA sleeping, student arrives
   - Student increments waiting, wakes TA
   - TA decrements waiting, helps student
   - Student gets help, leaves
   
   Scenario 2: TA busy, chairs available
   - Student increments waiting, sits
   - Waits for TA to signal availability
   - Gets help when turn comes
   
   Scenario 3: TA busy, all chairs full
   - Student checks chairs, finds all full
   - Doesn't increment waiting
   - Leaves immediately

9. STARVATION:
   - No starvation: FIFO queue ensures fairness
   - Each student eventually gets help
   - TA never starves (students finite)

10. PRACTICAL APPLICATIONS:
    - Help desk systems
    - Customer service queues
    - Resource allocation with waiting
    - Limited capacity systems

11. VARIANTS:
    - Multiple TAs (resource pool)
    - Priority students (priority queue)
    - Appointment system (scheduling)

12. COMMON MISTAKES:
    ✗ Not checking chairs before sitting
    ✗ Forgetting to decrement waiting
    ✗ Unlocking mutex while in critical section
    ✗ TA not signaling availability
==============================================================================
*/
