// Sender - Message Queue
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
using namespace std;

struct msg {
    long type;
    char text[100];
};

int main() {
    key_t key = ftok("msgqueue", 65);
    int msgid = msgget(key, IPC_CREAT | 0666);
    msg m;
    int msgType = 1;
    
    while(true) {
        cout << "Message: ";
        cin.getline(m.text, 100);
        m.type = msgType++;
        msgsnd(msgid, &m, sizeof(m.text), 0);
        if(strcmp(m.text, "exit") == 0) break;
    }
    return 0;
}

