/*
==============================================================================
ASSIGNMENT 4.4: READERS-WRITERS PROBLEM (READERS PRIORITY)
==============================================================================

PROBLEM:
- Multiple reader and writer threads
- Readers can read simultaneously
- Writers need exclusive access
- Priority to readers over writers
- Use mutex for reader count and semaphore for database

COMPILATION:
g++ 4_4.cpp -o 4_4 -lpthread

EXECUTION:
./4_4

KEY CONCEPTS:
1. Multiple readers allowed simultaneously
2. Writer needs exclusive access (no readers, no writers)
3. Readers have priority (may starve writers)
4. Reader count tracking

SYNCHRONIZATION:
- db_mutex: Protects database access (writer exclusive)
- reader_mutex: Protects reader count
- reader_count: Number of active readers

SAMPLE RUN:
Reader 1 reading...
Reader 2 reading... (simultaneous)
Writer 1 writing... (exclusive)
==============================================================================
*/

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define NUM_READERS 5
#define NUM_WRITERS 2

// ============================================================================
// SHARED VARIABLES
// ============================================================================
int shared_data = 0;            // Shared database
int reader_count = 0;           // Number of active readers
pthread_mutex_t db_mutex;       // Database access control
pthread_mutex_t reader_mutex;   // Reader count protection

// ============================================================================
// READER THREAD
// ============================================================================
void* reader(void* arg) {
    int reader_id = *(int*)arg;
    
    while(true) {
        // Entry section
        pthread_mutex_lock(&reader_mutex);
        reader_count++;
        if(reader_count == 1) {
            // First reader locks database
            pthread_mutex_lock(&db_mutex);
        }
        pthread_mutex_unlock(&reader_mutex);
        
        // === CRITICAL SECTION (READING) ===
        cout << "📖 Reader " << reader_id << " reading data: " << shared_data << endl;
        sleep(1);
        cout << "✓ Reader " << reader_id << " finished reading" << endl;
        // ==================================
        
        // Exit section
        pthread_mutex_lock(&reader_mutex);
        reader_count--;
        if(reader_count == 0) {
            // Last reader unlocks database
            pthread_mutex_unlock(&db_mutex);
        }
        pthread_mutex_unlock(&reader_mutex);
        
        sleep(2);   // Think time
    }
    
    return NULL;
}

// ============================================================================
// WRITER THREAD
// ============================================================================
void* writer(void* arg) {
    int writer_id = *(int*)arg;
    
    while(true) {
        // Entry section - exclusive access
        pthread_mutex_lock(&db_mutex);
        
        // === CRITICAL SECTION (WRITING) ===
        shared_data++;
        cout << "✍️  Writer " << writer_id << " writing data: " << shared_data << endl;
        sleep(2);
        cout << "✓ Writer " << writer_id << " finished writing" << endl;
        // ==================================
        
        // Exit section
        pthread_mutex_unlock(&db_mutex);
        
        sleep(3);   // Think time
    }
    
    return NULL;
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];
    
    cout << "========================================" << endl;
    cout << "  READERS-WRITERS PROBLEM" << endl;
    cout << "  (Readers Priority)" << endl;
    cout << "========================================" << endl;
    cout << "Readers: " << NUM_READERS << endl;
    cout << "Writers: " << NUM_WRITERS << endl;
    cout << "========================================\n" << endl;
    
    // Initialize mutexes
    pthread_mutex_init(&db_mutex, NULL);
    pthread_mutex_init(&reader_mutex, NULL);
    
    // Create reader threads
    for(int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }
    
    // Create writer threads
    for(int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }
    
    // Wait for threads (runs forever, use Ctrl+C to stop)
    for(int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for(int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }
    
    // Cleanup
    pthread_mutex_destroy(&db_mutex);
    pthread_mutex_destroy(&reader_mutex);
    
    return 0;
}

/*
==============================================================================
EXAM NOTES:

1. READERS-WRITERS PROBLEM:
   - Shared database accessed by readers and writers
   - Readers: Only read, don't modify (multiple allowed)
   - Writers: Modify data (exclusive access needed)

2. CONSTRAINTS:
   - Multiple readers can read simultaneously
   - Writer needs exclusive access (no readers, no writers)
   - Only one writer at a time

3. READERS PRIORITY SOLUTION:
   
   Reader Entry:
   lock(reader_mutex);
   reader_count++;
   if (reader_count == 1)
       lock(db_mutex);     // First reader locks database
   unlock(reader_mutex);
   READ DATA
   lock(reader_mutex);
   reader_count--;
   if (reader_count == 0)
       unlock(db_mutex);   // Last reader unlocks database
   unlock(reader_mutex);
   
   Writer Entry:
   lock(db_mutex);         // Get exclusive access
   WRITE DATA
   unlock(db_mutex);

4. WHY TWO MUTEXES:
   - db_mutex: Controls database access
   - reader_mutex: Protects reader_count variable

5. READER PRIORITY EXPLAINED:
   - Once a reader acquires db_mutex, other readers can join
   - Writer must wait until ALL readers finish
   - Problem: Writers may starve if readers keep coming

6. STARVATION:
   - Writers can starve (readers priority)
   - If readers arrive continuously, writer waits forever
   - Solution: Writers priority or fair scheduling

7. VARIANTS:
   a) Readers Priority (this solution):
      + No reader waits unless writer has db_mutex
      - Writers may starve
   
   b) Writers Priority:
      + No writer waits longer than necessary
      - Readers may wait even if no writer active
   
   c) Fair (FIFO):
      + No starvation
      - More complex implementation

8. EXAM SCENARIO:
   Time | Readers | Writer | State
   0    | R1      | -      | R1 locks db_mutex
   1    | R1,R2   | -      | R2 joins (no lock needed)
   2    | R1,R2   | W1     | W1 waits for readers
   3    | R2      | W1     | R1 done, R2 still reading
   4    | -       | W1     | R2 done, W1 gets lock

9. APPLICATIONS:
   - Database systems
   - File systems
   - Web caches
   - Any read-heavy system

10. KEY FORMULA:
    First reader: Locks database
    Last reader: Unlocks database
    Writer: Always locks/unlocks database
==============================================================================
*/
