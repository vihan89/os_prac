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
    int msgid = msgget(key, 0666);
    struct msg m;
    
    printf("Receiver waiting...\n");
    while(1) {
        msgrcv(msgid, &m, sizeof(m.text), 0, 0);
        for(int i = 0; m.text[i]; i++)
            if(m.text[i] >= 'a' && m.text[i] <= 'z')
                m.text[i] -= 32;
        printf("Received (uppercase): %s\n", m.text);
        
        if(strcmp(m.text, "EXIT") == 0) {
            printf("Exit command received. Shutting down...\n");
            break;
        }
    }
    
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
