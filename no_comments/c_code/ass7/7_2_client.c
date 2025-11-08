#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    
    printf("Client waiting for server...\n");
    while(1) {
        sem_wait(&shm->sem);
        if(strcmp(shm->msg, "EXIT") == 0) break;
        printf("Read: %s\n", shm->msg);
    }
    
    shmdt(shm);
    printf("Client exiting.\n");
    return 0;
}
