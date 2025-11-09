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
    int shmid = shmget(key, sizeof(struct SharedData), IPC_CREAT | 0666);
    struct SharedData *shm = (struct SharedData*) shmat(shmid, NULL, 0);
    
    sem_init(&shm->sem, 1, 0);
    
    printf("Server (type 'exit' to quit)\n");
    while(1) {
        printf("Write: ");
        if(fgets(shm->msg, 1000, stdin) == NULL) {
            printf("\nInput error or EOF. Exiting...\n");
            strcpy(shm->msg, "EXIT");
            sem_post(&shm->sem);
            break;
        }
        shm->msg[strcspn(shm->msg, "\n")] = 0;
        if(strcmp(shm->msg, "exit") == 0) {
            strcpy(shm->msg, "EXIT");
            sem_post(&shm->sem);
            break;
        }
        sem_post(&shm->sem);
        printf("Written!\n");
    }
    
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
