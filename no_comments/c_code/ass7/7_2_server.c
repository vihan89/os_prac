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
    int shmid = shmget(key, sizeof(struct SharedData), IPC_CREAT | 0666);
    struct SharedData *shm = (struct SharedData*) shmat(shmid, NULL, 0);
    
    sem_init(&shm->sem, 1, 0);
    
    while(1) {
        printf("Write: ");
        fgets(shm->msg, 1000, stdin);
        sem_post(&shm->sem);
    }
    
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
