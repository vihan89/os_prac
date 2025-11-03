/*
==============================================================================
ASSIGNMENT 7.2: IPC - SHARED MEMORY (CLIENT)
==============================================================================

PROBLEM:
- Implement client program using shared memory
- Read data from shared memory segment created by server
- Use System V IPC functions
- Remove shared memory after reading
- Synchronized with semaphores

COMPILATION:
g++ 7_2_client.cpp -o client -lpthread

EXECUTION:
1. Run server first: ./server
2. Run client: ./client

NOTE:
- Client and server must use SAME key
- Client removes shared memory after reading
- Multiple clients can read simultaneously
==============================================================================
*/

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <cstring>
#include <unistd.h>

using namespace std;

#define SHM_SIZE 1024

// Shared data structure (must match server's)
struct SharedData {
    char message[SHM_SIZE - sizeof(sem_t)];
    sem_t data_ready;
};

int main() {
    key_t key;
    int shmid;
    SharedData *shmaddr;
    
    cout << "========================================" << endl;
    cout << "  SHARED MEMORY - CLIENT" << endl;
    cout << "========================================\n" << endl;
    
    // Generate SAME key as server
    key = ftok(".", 'B');  // Same path and identifier
    
    if(key == -1) {
        perror("ftok failed");
        return 1;
    }
    
    cout << "Generated Key: " << key << endl;
    
    // Access existing shared memory segment
    // Don't use IPC_CREAT if server should create it
    shmid = shmget(key, SHM_SIZE, 0666);
    
    if(shmid == -1) {
        perror("shmget failed - Shared memory doesn't exist");
        cout << "\nMake sure server is running first!" << endl;
        return 1;
    }
    
    cout << "Shared Memory ID: " << shmid << endl;
    cout << "Connected to shared memory successfully!" << endl;
    
    // Attach shared memory
    shmaddr = (SharedData*) shmat(shmid, NULL, 0);
    
    if(shmaddr == (SharedData*) -1) {
        perror("shmat failed");
        return 1;
    }
    
    cout << "✓ Shared memory attached at address: " << (void*)shmaddr << endl;
    
    cout << "\n========================================" << endl;
    cout << "  READING FROM SHARED MEMORY" << endl;
    cout << "========================================" << endl;
    cout << "Waiting for server to write data...\n" << endl;
    
    int count = 0;
    char buffer[SHM_SIZE];
    
    while(true) {
        // Wait for server to signal data is ready
        cout << "Waiting for data..." << endl;
        sem_wait(&shmaddr->data_ready);
        
        // Read from shared memory
        strcpy(buffer, shmaddr->message);
        
        // Check if server exited
        if(strcmp(buffer, "SERVER_EXIT") == 0) {
            cout << "\nServer has exited. Stopping..." << endl;
            break;
        }
        
        // Display data
        count++;
        cout << "Read " << count << ": " << buffer << endl;
        cout << "  Length: " << strlen(buffer) << " bytes" << endl;
    }
    
    cout << "\n========================================" << endl;
    cout << "Total reads: " << count << endl;
    cout << "========================================" << endl;
    
    // Detach shared memory
    cout << "\nDetaching shared memory..." << endl;
    if(shmdt(shmaddr) == -1) {
        perror("shmdt failed");
    }
    else {
        cout << "✓ Shared memory detached successfully!" << endl;
    }
    
    // Remove shared memory segment
    cout << "\nRemoving shared memory segment..." << endl;
    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl failed");
        cout << "Warning: Could not remove shared memory" << endl;
        cout << "Remove manually using: ipcrm -m " << shmid << endl;
    }
    else {
        cout << "✓ Shared memory removed successfully!" << endl;
    }
    
    return 0;
}

/*
==============================================================================
EXAM NOTES:

1. CLIENT RESPONSIBILITIES:
   - Access shared memory created by server
   - Read data from shared memory
   - Detach when done
   - Remove shared memory (cleanup)

2. ACCESSING EXISTING SEGMENT:
   shmid = shmget(key, size, 0666);
   
   - No IPC_CREAT flag
   - Size should match server's
   - Returns existing shmid
   - Fails if segment doesn't exist

3. READING DATA:
   Method 1 - String:
   strcpy(buffer, shmaddr);
   
   Method 2 - Structure:
   struct Data *data = (struct Data*) shmaddr;
   int value = data->field;
   
   Method 3 - Array:
   int *arr = (int*) shmaddr;
   for(int i=0; i<n; i++) printf("%d", arr[i]);

4. SYNCHRONIZATION PROBLEM:
   Without sync:
   Client may read:
   - While server writing (partial data)
   - Before server writes (old/empty data)
   - After server overwrites (missed update)
   
   Solution: Use semaphores or flags

5. POLLING vs EVENT:
   Polling (shown in code):
   while(1) {
       read();
       sleep(2);
   }
   - Simple but inefficient
   - CPU cycles wasted
   
   Event-driven:
   Use semaphore:
   sem_wait(&data_ready);
   read();
   - Efficient (blocks until ready)

6. CLEANUP ORDER:
   1. Read all data
   2. shmdt() - Detach
   3. shmctl(IPC_RMID) - Remove
   
   Why this order?
   - Can't read after detach
   - Must detach before remove
   - Remove marks for deletion

7. MULTIPLE CLIENTS:
   - All can attach to same segment
   - All can read simultaneously
   - One removes, all must detach first
   - Last detach triggers actual deletion

8. ERROR SCENARIOS:
   Server not running:
   - shmget() fails with errno=ENOENT
   - Check server started first
   
   Permission denied:
   - Server created with restrictive perms
   - Use same user or chmod
   
   Already removed:
   - Another client removed
   - errno=EIDRM

9. SHARED MEMORY STATES:
   Created → Attached → In Use → Detached → Removed
   
   Multiple processes:
   Server: Create → Attach → Write → Detach
   Client1: Attach → Read → Detach
   Client2: Attach → Read → Detach → Remove

10. ADVANCED USAGE:
    Flag-based sync:
    
    struct SharedData {
        int ready_flag;
        char data[100];
    };
    
    Server:
    shm->ready_flag = 0;
    strcpy(shm->data, "Hello");
    shm->ready_flag = 1;  // Signal ready
    
    Client:
    while(shm->ready_flag == 0);  // Wait
    strcpy(buffer, shm->data);
    shm->ready_flag = 0;  // Acknowledge

11. EXAM WORKFLOW:
    1. ftok() with SAME params as server
    2. shmget() without IPC_CREAT
    3. shmat() to attach
    4. Read data (strcpy, memcpy, etc.)
    5. shmdt() to detach
    6. shmctl(IPC_RMID) to remove

12. COMPARISON: MESSAGE QUEUE vs SHARED MEMORY

    Message Queue:
    - Sender: msgsnd()
    - Receiver: msgrcv()
    - Kernel copies data
    - Built-in sync
    - Ordered
    
    Shared Memory:
    - Writer: strcpy(shm, data)
    - Reader: strcpy(data, shm)
    - No copying
    - Manual sync
    - No ordering

13. WHEN TO USE WHAT:
    Use Message Queue:
    - Small messages
    - Need sync/ordering
    - Producer-consumer
    
    Use Shared Memory:
    - Large data transfer
    - High performance needed
    - Multiple readers
    - Have sync mechanism

14. EXAM TIP:
    Must show:
    1. Same ftok() as server
    2. shmget() without IPC_CREAT
    3. shmat() to attach
    4. Read operation
    5. shmdt() before removal
    6. shmctl(IPC_RMID)

15. COMPLETE EXAMPLE:
    Server:
    key = ftok(".", 'X');
    shmid = shmget(key, 100, IPC_CREAT|0666);
    shm = (char*)shmat(shmid, NULL, 0);
    strcpy(shm, "Hello Client");
    shmdt(shm);
    
    Client:
    key = ftok(".", 'X');
    shmid = shmget(key, 100, 0666);
    shm = (char*)shmat(shmid, NULL, 0);
    printf("%s", shm);  // "Hello Client"
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);

16. DEBUGGING:
    Check segments: ipcs -m
    Details: ipcs -mi <shmid>
    Who's attached: ipcs -mp <shmid>
    Remove: ipcrm -m <shmid>

17. COMMON MISTAKES:
    - Different key than server
    - Using IPC_CREAT (creates new segment)
    - Reading before server writes
    - Not removing segment
    - Removing while others attached

18. SECURITY NOTE:
    - Any process with permissions can access
    - Use proper permissions (0600 for private)
    - Don't store sensitive data
    - Validate data before use

19. PERFORMANCE:
    Shared Memory: ~10 GB/s
    Message Queue: ~100 MB/s
    Pipe: ~1 GB/s
    Socket: ~100 MB/s
    
    Shared memory is fastest!

20. REAL-WORLD USE:
    - Database shared buffers
    - Graphics rendering (GPU-CPU)
    - Media streaming
    - High-frequency trading
    - Scientific computing
==============================================================================
*/
