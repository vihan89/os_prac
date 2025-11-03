

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

#define SHM_SIZE 1024

struct SharedData {
    char message[SHM_SIZE - sizeof(sem_t)];
    sem_t data_ready;
};

int main() {
    key_t key;
    int shmid;
    SharedData *shmaddr;
    
    cout << "========================================" << endl;
    cout << "  SHARED MEMORY - SERVER" << endl;
    cout << "========================================\n" << endl;
    

    key = ftok(".", 'B');
    
    if(key == -1) {
        perror("ftok failed");
        return 1;
    }
    
    cout << "Generated Key: " << key << endl;
    

    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    
    if(shmid == -1) {
        perror("shmget failed");
        return 1;
    }
    
    cout << "Shared Memory ID: " << shmid << endl;
    cout << "Shared memory segment created successfully!" << endl;
    cout << "Size: " << SHM_SIZE << " bytes" << endl;
    

    shmaddr = (SharedData*) shmat(shmid, NULL, 0);
    
    if(shmaddr == (SharedData*) -1) {
        perror("shmat failed");
        return 1;
    }
    
    cout << "✓ Shared memory attached at address: " << (void*)shmaddr << endl;
    

    if(sem_init(&shmaddr->data_ready, 1, 0) == -1) {
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
            strcpy(shmaddr->message, "SERVER_EXIT");
            sem_post(&shmaddr->data_ready);
            break;
        }
        

        strcpy(shmaddr->message, buffer);
        
        cout << "✓ Data written to shared memory!" << endl;
        cout << "  Length: " << strlen(buffer) << " bytes" << endl;
        

        sem_post(&shmaddr->data_ready);
        
        count++;
        sleep(1);
    }
    
    cout << "\n========================================" << endl;
    cout << "Total writes: " << count << endl;
    cout << "========================================" << endl;
    

    sem_destroy(&shmaddr->data_ready);
    

    cout << "\nDetaching shared memory..." << endl;
    if(shmdt(shmaddr) == -1) {
        perror("shmdt failed");
    }
    else {
        cout << "✓ Shared memory detached successfully!" << endl;
    }
    

    cout << "\nNote: Shared memory segment still exists." << endl;
    cout << "Client can still read the data." << endl;
    cout << "Client will remove it after reading." << endl;
    cout << "To remove manually: ipcrm -m " << shmid << endl;
    
    return 0;
}

