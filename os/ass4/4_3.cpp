/*
==============================================================================
ASSIGNMENT 4.3: PRODUCER-CONSUMER WITH FILE I/O
==============================================================================

PROBLEM:
- Producer writes random integers (0-9) to file
- Consumer reads integers from same file
- Use mutex for file access synchronization
- Use binary semaphore for coordination
- Handle file I/O properly

COMPILATION:
g++ 4_3.cpp -o 4_3 -lpthread

EXECUTION:
./4_3

KEY CONCEPTS:
1. Shared resource: File (not memory buffer)
2. Producer writes, Consumer reads
3. Mutual exclusion on file operations
4. Binary semaphore for data availability

SAMPLE RUN:
Producer writes: 5
Consumer reads: 5
Producer writes: 3
Consumer reads: 3
...
==============================================================================
*/

#include <iostream>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;

#define FILENAME "shared_data.txt"
#define NUM_ITEMS 10

// ============================================================================
// SHARED VARIABLES
// ============================================================================
sem_t data_available;           // Signals data is written
pthread_mutex_t file_mutex;     // Protects file access

// ============================================================================
// PRODUCER THREAD - WRITES TO FILE
// ============================================================================
void* producer(void* arg) {
    srand(time(NULL));
    
    for(int i = 0; i < NUM_ITEMS; i++) {
        int data = rand() % 10;     // Generate random 0-9
        
        // Lock file access
        pthread_mutex_lock(&file_mutex);
        
        // Write to file
        ofstream file(FILENAME);
        if(file.is_open()) {
            file << data;
            file.close();
            cout << "📝 Producer writes: " << data << endl;
        }
        else {
            cout << "❌ Producer: Error opening file!" << endl;
        }
        
        pthread_mutex_unlock(&file_mutex);
        
        // Signal that data is available
        sem_post(&data_available);
        
        sleep(1);
    }
    
    cout << "\n✓ Producer finished writing." << endl;
    return NULL;
}

// ============================================================================
// CONSUMER THREAD - READS FROM FILE
// ============================================================================
void* consumer(void* arg) {
    for(int i = 0; i < NUM_ITEMS; i++) {
        // Wait for data to be available
        sem_wait(&data_available);
        
        // Lock file access
        pthread_mutex_lock(&file_mutex);
        
        // Read from file
        ifstream file(FILENAME);
        if(file.is_open()) {
            int data;
            file >> data;
            file.close();
            cout << "📖 Consumer reads: " << data << endl;
        }
        else {
            cout << "❌ Consumer: Error opening file!" << endl;
        }
        
        pthread_mutex_unlock(&file_mutex);
        
        sleep(2);
    }
    
    cout << "\n✓ Consumer finished reading." << endl;
    return NULL;
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    pthread_t prod, cons;
    
    cout << "========================================" << endl;
    cout << "  PRODUCER-CONSUMER WITH FILE I/O" << endl;
    cout << "========================================" << endl;
    cout << "File: " << FILENAME << endl;
    cout << "Items: " << NUM_ITEMS << endl;
    cout << "========================================\n" << endl;
    
    // Initialize synchronization
    sem_init(&data_available, 0, 0);
    pthread_mutex_init(&file_mutex, NULL);
    
    // Create threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    
    // Wait for completion
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    
    // Cleanup
    sem_destroy(&data_available);
    pthread_mutex_destroy(&file_mutex);
    
    cout << "\n========================================" << endl;
    cout << "  OPERATIONS COMPLETED!" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

/*
==============================================================================
EXAM NOTES:

1. FILE AS SHARED RESOURCE:
   - File replaces shared memory buffer
   - Only one thread should access file at a time
   - Use mutex for mutual exclusion

2. KEY DIFFERENCES FROM 4.1:
   - No bounded buffer (just single value in file)
   - File I/O instead of memory operations
   - Simpler: no empty/full counting needed

3. SYNCHRONIZATION:
   Mutex: Prevents simultaneous file access
   Semaphore: Signals data availability
   
4. PRODUCER:
   lock(file_mutex);
   write_to_file(data);
   unlock(file_mutex);
   signal(data_available);

5. CONSUMER:
   wait(data_available);
   lock(file_mutex);
   read_from_file(data);
   unlock(file_mutex);

6. FILE OPERATIONS:
   ofstream: Output file stream (write)
   ifstream: Input file stream (read)
   Always check if file opened successfully
   Close file after operations
==============================================================================
*/
