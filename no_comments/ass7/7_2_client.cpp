#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <cstring>
using namespace std;

struct SharedData {
    char msg[1000];
    sem_t sem;
};

int main() {
    key_t key = ftok(".", 'B');
    int shmid = shmget(key, sizeof(SharedData), 0666);
    SharedData *shm = (SharedData*) shmat(shmid, NULL, 0);
    
    cout << "Client waiting for server..." << endl;
    while(true) {
        sem_wait(&shm->sem);
        if(strcmp(shm->msg, "EXIT") == 0) break;
        cout << "Read: " << shm->msg << endl;
    }
    
    shmdt(shm);
    cout << "Client exiting." << endl;
    return 0;
}

