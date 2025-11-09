// Receiver - Convert to uppercase
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <cctype>
using namespace std;

struct msg {
    long type;
    char text[100];
};

int main() {
    key_t key = ftok("msgqueue", 65);
    int msgid = msgget(key, 0666);
    msg m;
    
    while(true) {
        msgrcv(msgid, &m, sizeof(m.text), 0, 0);
        for(int i = 0; m.text[i]; i++)
            m.text[i] = toupper(m.text[i]);
        cout << "Message received: " << m.text << "\n";
        if(strcmp(m.text, "EXIT") == 0) break;
    }
    
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}

