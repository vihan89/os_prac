/*
==============================================================================
ASSIGNMENT 4.1: PRODUCER-CONSUMER (PIZZA PROBLEM)
==============================================================================

PROBLEM:
- Simpson (producer) bakes pizzas
- Joey (consumer) eats pizzas  
- Shared buffer with fixed size (5)
- Use counting semaphores for empty and full slots
- Use mutex for buffer access
- Prevent race conditions

COMPILATION:
g++ 4_1.cpp -o 4_1 -lpthread

EXECUTION:
./4_1

KEY CONCEPTS:
1. Producer: Creates items and puts in buffer
2. Consumer: Takes items from buffer
3. Bounded Buffer: Fixed size shared buffer
4. Semaphore: Synchronization primitive for counting resources
5. Mutex: Mutual exclusion lock for critical section

SEMAPHORES USED:
- empty: Counts empty slots (initially BUFFER_SIZE)
- full: Counts full slots (initially 0)
- mutex: Protects buffer access (binary semaphore)

OPERATIONS:
- sem_wait(): Decrement (P operation, wait if 0)
- sem_post(): Increment (V operation, signal)

SAMPLE RUN:
Simpson bakes pizza 1
Joey eats pizza 1
Simpson bakes pizza 2
...
==============================================================================
*/

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>     // sleep()

using namespace std;

#define BUFFER_SIZE 5
#define NUM_ITEMS 10

// ============================================================================
// SHARED VARIABLES
// ============================================================================
int buffer[BUFFER_SIZE];    // Shared buffer for pizzas
int in = 0;                 // Index for next production
int out = 0;                // Index for next consumption

// Semaphores
sem_t empty;                // Counts empty slots
sem_t full;                 // Counts full slots
pthread_mutex_t mutex;      // Mutex for buffer access

// ============================================================================
// PRODUCER THREAD (SIMPSON BAKES PIZZAS)
// ============================================================================
void* producer(void* arg) {
    for(int i = 1; i <= NUM_ITEMS; i++) {
        // Wait if buffer is full
        sem_wait(&empty);       // P(empty) - decrement empty count
        
        // Lock buffer for exclusive access
        pthread_mutex_lock(&mutex);
        
        // ===== CRITICAL SECTION =====
        buffer[in] = i;         // Produce pizza with number i
        cout << "🍕 Simpson bakes pizza " << i << " (placed at slot " << in << ")" << endl;
        in = (in + 1) % BUFFER_SIZE;    // Circular buffer
        // ===========================
        
        // Unlock buffer
        pthread_mutex_unlock(&mutex);
        
        // Signal that buffer has one more full slot
        sem_post(&full);        // V(full) - increment full count
        
        sleep(1);               // Simulate baking time
    }
    
    cout << "\n✓ Simpson finished baking all pizzas!" << endl;
    return NULL;
}

// ============================================================================
// CONSUMER THREAD (JOEY EATS PIZZAS)
// ============================================================================
void* consumer(void* arg) {
    for(int i = 1; i <= NUM_ITEMS; i++) {
        // Wait if buffer is empty
        sem_wait(&full);        // P(full) - decrement full count
        
        // Lock buffer for exclusive access
        pthread_mutex_lock(&mutex);
        
        // ===== CRITICAL SECTION =====
        int pizza = buffer[out];    // Consume pizza
        cout << "🍴 Joey eats pizza " << pizza << " (from slot " << out << ")" << endl;
        out = (out + 1) % BUFFER_SIZE;  // Circular buffer
        // ===========================
        
        // Unlock buffer
        pthread_mutex_unlock(&mutex);
        
        // Signal that buffer has one more empty slot
        sem_post(&empty);       // V(empty) - increment empty count
        
        sleep(2);               // Simulate eating time
    }
    
    cout << "\n✓ Joey finished eating all pizzas!" << endl;
    return NULL;
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    pthread_t prod_thread, cons_thread;
    
    cout << "========================================" << endl;
    cout << "  PRODUCER-CONSUMER PROBLEM" << endl;
    cout << "  (Simpson Bakes, Joey Eats)" << endl;
    cout << "========================================" << endl;
    cout << "Buffer Size: " << BUFFER_SIZE << " pizzas" << endl;
    cout << "Total Pizzas: " << NUM_ITEMS << endl;
    cout << "========================================\n" << endl;
    
    // ========================================================================
    // INITIALIZE SYNCHRONIZATION PRIMITIVES
    // ========================================================================
    
    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);   // Initially all slots empty
    sem_init(&full, 0, 0);              // Initially no slots full
    
    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);
    
    // ========================================================================
    // CREATE THREADS
    // ========================================================================
    
    // Create producer thread (Simpson)
    pthread_create(&prod_thread, NULL, producer, NULL);
    
    // Create consumer thread (Joey)
    pthread_create(&cons_thread, NULL, consumer, NULL);
    
    // ========================================================================
    // WAIT FOR THREADS TO COMPLETE
    // ========================================================================
    
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);
    
    // ========================================================================
    // CLEANUP
    // ========================================================================
    
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    
    cout << "\n========================================" << endl;
    cout << "  ALL OPERATIONS COMPLETED!" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

/*
==============================================================================
IMPORTANT NOTES FOR EXAM:

1. PRODUCER-CONSUMER PROBLEM:
   - Classic synchronization problem
   - Producer produces items → buffer
   - Consumer consumes items ← buffer
   - Buffer is bounded (fixed size)
   - Need synchronization to prevent:
     * Producer writing to full buffer
     * Consumer reading from empty buffer
     * Race conditions on buffer

2. SEMAPHORE:
   - Integer variable for synchronization
   - Two atomic operations: P (wait) and V (signal)
   - P (wait): Decrement, block if 0
   - V (signal): Increment, wake up waiting process
   - Counting semaphore: Can have any value ≥ 0
   - Binary semaphore: Only 0 or 1

3. SOLUTION COMPONENTS:
   
   Semaphore 'empty':
   - Initial value = BUFFER_SIZE
   - Counts number of empty slots
   - Producer waits on 'empty'
   - Producer signals 'full'
   
   Semaphore 'full':
   - Initial value = 0
   - Counts number of full slots
   - Consumer waits on 'full'
   - Consumer signals 'empty'
   
   Mutex:
   - Ensures mutual exclusion
   - Only one thread accesses buffer at a time
   - Protects critical section

4. PRODUCER PSEUDOCODE:
   while (true) {
       produce_item();
       wait(empty);        // Wait for empty slot
       lock(mutex);        // Enter critical section
       add_to_buffer();
       unlock(mutex);      // Exit critical section
       signal(full);       // Signal full slot available
   }

5. CONSUMER PSEUDOCODE:
   while (true) {
       wait(full);         // Wait for full slot
       lock(mutex);        // Enter critical section
       remove_from_buffer();
       unlock(mutex);      // Exit critical section
       signal(empty);      // Signal empty slot available
       consume_item();
   }

6. CIRCULAR BUFFER:
   - Array with fixed size
   - 'in' pointer: next production position
   - 'out' pointer: next consumption position
   - Wrap around using modulo: (index + 1) % SIZE
   - Buffer full: (in + 1) % SIZE == out
   - Buffer empty: in == out

7. PTHREAD FUNCTIONS:
   - pthread_create(): Create new thread
   - pthread_join(): Wait for thread termination
   - pthread_mutex_init(): Initialize mutex
   - pthread_mutex_lock(): Acquire mutex
   - pthread_mutex_unlock(): Release mutex
   - pthread_mutex_destroy(): Destroy mutex

8. SEMAPHORE FUNCTIONS:
   - sem_init(&sem, pshared, value): Initialize
     * pshared=0: thread-shared
     * pshared=1: process-shared
     * value: initial value
   - sem_wait(&sem): P operation (decrement)
   - sem_post(&sem): V operation (increment)
   - sem_destroy(&sem): Destroy semaphore

9. RACE CONDITION:
   - Multiple threads access shared data
   - At least one modifies data
   - Outcome depends on execution timing
   - Solution: Use synchronization (mutex/semaphore)

10. CRITICAL SECTION:
    - Code segment accessing shared resource
    - Must execute atomically
    - Only one thread at a time
    - Protected by mutex

11. WHY THREE SYNCHRONIZATION OBJECTS?
    - empty: Prevents producer from adding to full buffer
    - full: Prevents consumer from removing from empty buffer
    - mutex: Prevents simultaneous buffer access

12. EXAM TIP - REMEMBER THE PATTERN:
    Producer:                Consumer:
    1. wait(empty)          1. wait(full)
    2. lock(mutex)          2. lock(mutex)
    3. add to buffer        3. remove from buffer
    4. unlock(mutex)        4. unlock(mutex)
    5. signal(full)         5. signal(empty)

13. COMMON ERRORS TO AVOID:
    ✗ Forgetting to unlock mutex
    ✗ Wrong semaphore order
    ✗ Not initializing semaphores
    ✗ Deadlock: both wait on same semaphore
    
14. COMPILATION NOTE:
    - Must link pthread library: -lpthread
    - Example: g++ program.cpp -o program -lpthread
==============================================================================
*/
