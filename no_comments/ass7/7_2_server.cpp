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
    int shmid = shmget(key, sizeof(SharedData), IPC_CREAT | 0666);
    SharedData *shm = (SharedData*) shmat(shmid, NULL, 0);
    
    sem_init(&shm->sem, 1, 0);
    
    cout << "Server (type 'exit' to quit)" << endl;
    while(true) {
        cout << "Write: ";
        if(!cin.getline(shm->msg, 1000)) {
            cout << "\nInput error or EOF. Exiting..." << endl;
            strcpy(shm->msg, "EXIT");
            sem_post(&shm->sem);
            break;
        }
        if(strcmp(shm->msg, "exit") == 0) {
            strcpy(shm->msg, "EXIT");
            sem_post(&shm->sem);
            break;
        }
        sem_post(&shm->sem);
        cout << "Written!" << endl;
    }
    
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

