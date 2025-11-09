#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

struct SharedData {
    char msg[1000];
    sem_t sem;
};

int main() {
    key_t key = ftok(".", 'B');
    int shmid = shmget(key, sizeof(struct SharedData), 0666);
    struct SharedData *shm = (struct SharedData*) shmat(shmid, NULL, 0);
    
    while(1) {
        sem_wait(&shm->sem);
        printf("Read: %s\n", shm->msg);
    }
    
    shmdt(shm);
    return 0;
}
