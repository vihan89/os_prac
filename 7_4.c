/*
7.4 Shared Memory: 
Develop Client and Server processes using Shared Memory. The Server creates a 
shared memory segment and writes a message to it. The Client reads the message 
from the shared memory segment and displays it on the screen. Include appropriate 
synchronization (using semaphores or sleep/wait) to avoid data inconsistency.

Compile and run:
gcc 7_4.c -o 7_4
./7_4 server    # Run in one terminal
./7_4 client    # Run in another terminal
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define SHM_KEY 5678
#define SEM_KEY 9012
#define SHM_SIZE 1024

// Semaphore operations
struct sembuf sem_lock = {0, -1, 0};    // Wait (P operation)
struct sembuf sem_unlock = {0, 1, 0};   // Signal (V operation)

// Shared memory structure
struct shared_data {
    int ready;           // Flag to indicate data is ready
    char message[SHM_SIZE];
};

// Server process
void server() {
    int shmid, semid;
    struct shared_data *shm;
    char input[SHM_SIZE];
    
    printf("========================================\n");
    printf("  SHARED MEMORY - SERVER\n");
    printf("========================================\n\n");
    
    // Create shared memory segment
    shmid = shmget(SHM_KEY, sizeof(struct shared_data), 0666 | IPC_CREAT);
    if(shmid == -1) {
        perror("shmget failed");
        exit(1);
    }
    printf("Shared Memory created with ID: %d\n", shmid);
    
    // Create semaphore for synchronization
    semid = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
    if(semid == -1) {
        perror("semget failed");
        exit(1);
    }
    printf("Semaphore created with ID: %d\n\n", semid);
    
    // Initialize semaphore to 1 (unlocked)
    if(semctl(semid, 0, SETVAL, 1) == -1) {
        perror("semctl failed");
        exit(1);
    }
    
    // Attach to shared memory
    shm = (struct shared_data *) shmat(shmid, NULL, 0);
    if(shm == (struct shared_data *) -1) {
        perror("shmat failed");
        exit(1);
    }
    
    while(1) {
        printf("Enter message (type 'exit' to quit): ");
        fgets(input, SHM_SIZE, stdin);
        input[strcspn(input, "\n")] = '\0';
        
        if(strcmp(input, "exit") == 0) {
            break;
        }
        
        // Lock semaphore
        printf("[Server] Acquiring lock...\n");
        if(semop(semid, &sem_lock, 1) == -1) {
            perror("semop lock failed");
            exit(1);
        }
        
        // Write to shared memory
        printf("[Server] Writing to shared memory...\n");
        strcpy(shm->message, input);
        shm->ready = 1;  // Set ready flag
        
        printf("[Server] Message written: \"%s\"\n", shm->message);
        
        // Unlock semaphore
        printf("[Server] Releasing lock...\n");
        if(semop(semid, &sem_unlock, 1) == -1) {
            perror("semop unlock failed");
            exit(1);
        }
        
        printf("[Server] Waiting for client to read...\n\n");
        
        // Wait for client to read (ready flag becomes 0)
        while(shm->ready == 1) {
            sleep(1);
        }
    }
    
    // Cleanup
    printf("\n[Server] Cleaning up...\n");
    
    // Detach shared memory
    if(shmdt(shm) == -1) {
        perror("shmdt failed");
        exit(1);
    }
    
    // Remove shared memory
    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl failed");
        exit(1);
    }
    
    // Remove semaphore
    if(semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl remove failed");
        exit(1);
    }
    
    printf("[Server] Shared memory and semaphore removed.\n");
    printf("[Server] Exiting...\n");
}

// Client process
void client() {
    int shmid, semid;
    struct shared_data *shm;
    
    printf("========================================\n");
    printf("  SHARED MEMORY - CLIENT\n");
    printf("========================================\n\n");
    
    printf("Waiting for server to create shared memory...\n");
    sleep(2);  // Give server time to create
    
    // Get existing shared memory segment
    shmid = shmget(SHM_KEY, sizeof(struct shared_data), 0666);
    if(shmid == -1) {
        perror("shmget failed - Server not running?");
        exit(1);
    }
    printf("Connected to Shared Memory ID: %d\n", shmid);
    
    // Get existing semaphore
    semid = semget(SEM_KEY, 1, 0666);
    if(semid == -1) {
        perror("semget failed");
        exit(1);
    }
    printf("Connected to Semaphore ID: %d\n\n", semid);
    
    // Attach to shared memory
    shm = (struct shared_data *) shmat(shmid, NULL, 0);
    if(shm == (struct shared_data *) -1) {
        perror("shmat failed");
        exit(1);
    }
    
    printf("Waiting for messages from server...\n");
    printf("(Press Ctrl+C to stop)\n\n");
    
    while(1) {
        // Wait for data to be ready
        if(shm->ready == 1) {
            // Lock semaphore
            printf("[Client] Acquiring lock...\n");
            if(semop(semid, &sem_lock, 1) == -1) {
                perror("semop lock failed");
                exit(1);
            }
            
            // Read from shared memory
            printf("[Client] Reading from shared memory...\n");
            
            printf("========================================\n");
            printf("  MESSAGE FROM SERVER\n");
            printf("========================================\n");
            printf("%s\n", shm->message);
            printf("========================================\n\n");
            
            // Mark as read
            shm->ready = 0;
            
            // Unlock semaphore
            printf("[Client] Releasing lock...\n\n");
            if(semop(semid, &sem_unlock, 1) == -1) {
                perror("semop unlock failed");
                exit(1);
            }
        }
        
        sleep(1);  // Avoid busy waiting
    }
    
    // Detach shared memory
    if(shmdt(shm) == -1) {
        perror("shmdt failed");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s [server|client]\n", argv[0]);
        printf("\nExamples:\n");
        printf("  %s server  - Run as server process\n", argv[0]);
        printf("  %s client  - Run as client process\n", argv[0]);
        printf("\nNote: Run server first, then client in another terminal\n");
        return 1;
    }
    
    if(strcmp(argv[1], "server") == 0) {
        server();
    } else if(strcmp(argv[1], "client") == 0) {
        client();
    } else {
        printf("Invalid argument! Use 'server' or 'client'\n");
        return 1;
    }
    
    return 0;
}
