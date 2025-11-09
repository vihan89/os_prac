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
    int shmid = shmget(key, sizeof(SharedData), 0666);
    SharedData *shm = (SharedData*) shmat(shmid, NULL, 0);
    
    sem_wait(&shm->sem);
    cout << "Read: " << shm->msg << endl;
    
    shmdt(shm);
    return 0;
}

