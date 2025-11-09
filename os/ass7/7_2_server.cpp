/*
==============================================================================
ASSIGNMENT 7.2: IPC - SHARED MEMORY (SERVER)
==============================================================================

PROBLEM:
- Implement server program using shared memory
- Write data to shared memory segment
- Use System V IPC: shmget(), shmat(), shmdt(), shmctl()
- Client reads from same shared memory
- Include synchronization using semaphores

COMPILATION:
g++ 7_2_server.cpp -o server -lpthread

EXECUTION:
1. Run server: ./server
2. Run client (in another terminal): ./client

SHARED MEMORY FUNCTIONS:
1. shmget(key, size, flags): Create/access shared memory
2. shmat(shmid, addr, flags): Attach to process address space
3. shmdt(shmaddr): Detach from process
4. shmctl(shmid, IPC_RMID, NULL): Remove shared memory

KEY ADVANTAGE:
- Fastest IPC (direct memory access)
- No kernel copying
- Both processes access same physical memory
==============================================================================
*/

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

#define SHM_SIZE 1024  // Shared memory size in bytes

// Shared data structure with synchronization
struct SharedData {
    char message[SHM_SIZE - sizeof(sem_t)];
    sem_t data_ready;  // Semaphore for synchronization
};

int main() {
    key_t key;
    int shmid;
    SharedData *shmaddr;
    
    cout << "========================================" << endl;
    cout << "  SHARED MEMORY - SERVER" << endl;
    cout << "========================================\n" << endl;
    
    // Generate unique key
    key = ftok(".", 'B');  // Use 'B' to distinguish from message queue
    
    if(key == -1) {
        perror("ftok failed");
        return 1;
    }
    
    cout << "Generated Key: " << key << endl;
    
    // Create shared memory segment
    // shmget(key, size, flags)
    // Size: Number of bytes
    // IPC_CREAT | 0666: Create with read/write permissions
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    
    if(shmid == -1) {
        perror("shmget failed");
        return 1;
    }
    
    cout << "Shared Memory ID: " << shmid << endl;
    cout << "Shared memory segment created successfully!" << endl;
    cout << "Size: " << SHM_SIZE << " bytes" << endl;
    
    // Attach shared memory to process address space
    // shmat(shmid, NULL, 0)
    // NULL: Kernel chooses address
    // 0: Read/write mode (SHM_RDONLY for read-only)
    shmaddr = (SharedData*) shmat(shmid, NULL, 0);
    
    if(shmaddr == (SharedData*) -1) {
        perror("shmat failed");
        return 1;
    }
    
    cout << "✓ Shared memory attached at address: " << (void*)shmaddr << endl;
    
    // Initialize semaphore
    if(sem_init(&shmaddr->data_ready, 1, 0) == -1) {  // 1 = shared between processes, 0 = initial value
        perror("sem_init failed");
        return 1;
    }
    cout << "✓ Semaphore initialized for synchronization" << endl;
    
    cout << "\n========================================" << endl;
    cout << "  WRITING TO SHARED MEMORY" << endl;
    cout << "========================================" << endl;
    cout << "Enter data to write (type 'exit' to quit)" << endl;
    cout << "Client can read this data\n" << endl;
    
    char buffer[SHM_SIZE];
    int count = 0;
    
    while(true) {
        cout << "\nWrite " << (count + 1) << ": ";
        cin.getline(buffer, SHM_SIZE);
        
        if(strcmp(buffer, "exit") == 0) {
            cout << "\nExiting server..." << endl;
            strcpy(shmaddr->message, "SERVER_EXIT");  // Signal to client
            sem_post(&shmaddr->data_ready);  // Signal client
            break;
        }
        
        // Write to shared memory
        strcpy(shmaddr->message, buffer);
        
        cout << "✓ Data written to shared memory!" << endl;
        cout << "  Length: " << strlen(buffer) << " bytes" << endl;
        
        // Signal client that data is ready
        sem_post(&shmaddr->data_ready);
        
        count++;
        sleep(1);
    }
    
    cout << "\n========================================" << endl;
    cout << "Total writes: " << count << endl;
    cout << "========================================" << endl;
    
    // Destroy semaphore
    sem_destroy(&shmaddr->data_ready);
    
    // Detach shared memory
    cout << "\nDetaching shared memory..." << endl;
    if(shmdt(shmaddr) == -1) {
        perror("shmdt failed");
    }
    else {
        cout << "✓ Shared memory detached successfully!" << endl;
    }
    
    // Note: Not removing shared memory here
    // Client may still need to read
    cout << "\nNote: Shared memory segment still exists." << endl;
    cout << "Client can still read the data." << endl;
    cout << "Client will remove it after reading." << endl;
    cout << "To remove manually: ipcrm -m " << shmid << endl;
    
    return 0;
}

/*
==============================================================================
EXAM NOTES:

1. SHARED MEMORY BASICS:
   - Fastest IPC mechanism
   - Multiple processes share same memory region
   - Direct memory access (no copying)
   - Needs synchronization (semaphores/mutex)
   - Persists until explicitly removed

2. SHARED MEMORY FUNCTIONS:

   shmget(key, size, flags):
   - Creates or accesses shared memory segment
   - key: From ftok() or IPC_PRIVATE
   - size: Size in bytes (rounded to page size)
   - flags: IPC_CREAT | permissions
   - Returns: shmid (shared memory ID)
   
   shmat(shmid, addr, flags):
   - Attaches shared memory to process
   - shmid: From shmget()
   - addr: NULL (kernel chooses) or specific address
   - flags: 0 (RW) or SHM_RDONLY
   - Returns: Pointer to shared memory
   
   shmdt(shmaddr):
   - Detaches shared memory from process
   - Doesn't delete segment
   - Process can't access after detach
   
   shmctl(shmid, cmd, buf):
   - Control operations
   - IPC_RMID: Remove segment
   - IPC_STAT: Get status
   - IPC_SET: Set attributes

3. WORKFLOW:
   Server:
   1. ftok() → key
   2. shmget() → shmid (create)
   3. shmat() → shmaddr (attach)
   4. Write data: strcpy(shmaddr, data)
   5. shmdt() (detach)
   6. Keep segment for client
   
   Client:
   1. ftok() → key (SAME as server)
   2. shmget() → shmid (access)
   3. shmat() → shmaddr (attach)
   4. Read data: strcpy(data, shmaddr)
   5. shmdt() (detach)
   6. shmctl(IPC_RMID) (remove)

4. KEY GENERATION:
   key = ftok(".", 'B');
   - Uses current directory
   - 'B' is project identifier
   - Both server and client must use SAME parameters

5. SIZE PARAMETER:
   - Specify in bytes
   - Kernel rounds to page size (usually 4KB)
   - Requesting 1024 → Gets 4096
   - Check actual size: shmctl(IPC_STAT)

6. ATTACHMENT:
   shmaddr = (char*) shmat(shmid, NULL, 0);
   
   - NULL: Kernel chooses address
   - Specify address: shmat(shmid, (void*)0x5000, 0)
   - SHM_RDONLY: Read-only attachment
   - Returns (void*)-1 on error

7. WRITING/READING:
   Write: strcpy(shmaddr, "Hello");
   Read:  strcpy(buffer, shmaddr);
   
   - Treat as normal pointer
   - Can use any data type
   - No bounds checking
   - Manual synchronization needed

8. SYNCHRONIZATION ISSUE:
   Without sync:
   - Writer may overwrite before reader reads
   - Reader may read partial data
   
   Solutions:
   - Semaphores (sem_wait/sem_post)
   - Flags in shared memory
   - Message queues for signaling

9. CLEANUP:
   Detach: shmdt(shmaddr)
   - Process can't access anymore
   - Segment still exists
   - Other processes can still use
   
   Remove: shmctl(shmid, IPC_RMID, NULL)
   - Marks for deletion
   - Deleted when all processes detach
   - System resources freed

10. SYSTEM LIMITS:
    View limits: ipcs -lm
    - SHMMAX: Max segment size (~2GB)
    - SHMMIN: Min segment size (1 byte)
    - SHMMNI: Max segments system-wide
    - SHMSEG: Max segments per process

11. DEBUGGING COMMANDS:
    View segments: ipcs -m
    Segment details: ipcs -mi <shmid>
    Remove: ipcrm -m <shmid>
    Remove all: ipcrm --all=shm

12. ADVANTAGES:
    ✓ Fastest IPC (direct access)
    ✓ Good for large data
    ✓ Multiple readers/writers
    ✓ No kernel involvement after attach
    
13. DISADVANTAGES:
    ✗ No built-in synchronization
    ✗ Manual cleanup needed
    ✗ Security concerns (any process can access)
    ✗ Not suitable across network

14. EXAMPLE WITH STRUCTURE:
    struct Data {
        int id;
        char name[50];
    };
    
    Data *shmaddr = (Data*) shmat(shmid, NULL, 0);
    shmaddr->id = 123;
    strcpy(shmaddr->name, "Test");

15. EXAM TIP:
    Must show:
    1. ftok() for key
    2. shmget() with size and IPC_CREAT
    3. shmat() with NULL address
    4. Write operation
    5. shmdt() before exit
    6. Mention who removes (usually client)

16. COMMON ERRORS:
    - Different keys in server/client
    - Size too small for data
    - Forgetting to detach
    - Not removing segment
    - No synchronization
    - Writing beyond size

17. MEMORY LAYOUT:
    Virtual Memory:
    [Text][Data][Heap][Shared Memory][Stack]
                       ↑
                  Attached here
    
    Same physical memory mapped to multiple processes

18. COMPARISON WITH MESSAGE QUEUE:
    Shared Memory:
    - Faster (no copying)
    - Manual sync needed
    - Good for large data
    
    Message Queue:
    - Built-in sync
    - Automatic ordering
    - Size limited
==============================================================================
*/
