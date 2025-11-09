#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg {
    long type;
    char text[512];
};

int main() {
    key_t key = ftok(".", 'A');
    int msgid = msgget(key, IPC_CREAT | 0666);
    struct msg m;
    m.type = 1;
    
    printf("Sender (type 'exit' to quit)\n");
    while(1) {
        printf("Message: ");
        if(fgets(m.text, 512, stdin) == NULL) {
            printf("\nInput error or EOF. Exiting...\n");
            break;
        }
        m.text[strcspn(m.text, "\n")] = 0;
        if(strcmp(m.text, "exit") == 0) break;
        msgsnd(msgid, &m, sizeof(m.text), 0);
        printf("Sent!\n");
    }
    
    return 0;
}
