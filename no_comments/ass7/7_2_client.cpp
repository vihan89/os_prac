

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <cstring>
#include <unistd.h>

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
    cout << "  SHARED MEMORY - CLIENT" << endl;
    cout << "========================================\n" << endl;
    

    key = ftok(".", 'B');
    
    if(key == -1) {
        perror("ftok failed");
        return 1;
    }
    
    cout << "Generated Key: " << key << endl;
    

    shmid = shmget(key, SHM_SIZE, 0666);
    
    if(shmid == -1) {
        perror("shmget failed - Shared memory doesn't exist");
        cout << "\nMake sure server is running first!" << endl;
        return 1;
    }
    
    cout << "Shared Memory ID: " << shmid << endl;
    cout << "Connected to shared memory successfully!" << endl;
    

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

        cout << "Waiting for data..." << endl;
        sem_wait(&shmaddr->data_ready);
        

        strcpy(buffer, shmaddr->message);
        

        if(strcmp(buffer, "SERVER_EXIT") == 0) {
            cout << "\nServer has exited. Stopping..." << endl;
            break;
        }
        

        count++;
        cout << "Read " << count << ": " << buffer << endl;
        cout << "  Length: " << strlen(buffer) << " bytes" << endl;
    }
    
    cout << "\n========================================" << endl;
    cout << "Total reads: " << count << endl;
    cout << "========================================" << endl;
    

    cout << "\nDetaching shared memory..." << endl;
    if(shmdt(shmaddr) == -1) {
        perror("shmdt failed");
    }
    else {
        cout << "✓ Shared memory detached successfully!" << endl;
    }
    

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

