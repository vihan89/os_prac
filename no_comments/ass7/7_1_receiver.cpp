#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
using namespace std;

struct msg {
    long type;
    char text[512];
};

int main() {
    key_t key = ftok(".", 'A');
    int msgid = msgget(key, 0666);
    msg m;
    
    cout << "Receiver waiting..." << endl;
    while(true) {
        msgrcv(msgid, &m, sizeof(m.text), 0, 0);
        for(int i = 0; m.text[i]; i++)
            if(m.text[i] >= 'a' && m.text[i] <= 'z')
                m.text[i] -= 32;
        cout << "Received (uppercase): " << m.text << endl;
    }
    
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}

