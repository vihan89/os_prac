#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
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
    
    while(true) {
        cout << "Write: ";
        cin.getline(shm->msg, 1000);
        sem_post(&shm->sem);
    }
    
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

