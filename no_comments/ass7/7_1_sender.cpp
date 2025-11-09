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
    int msgid = msgget(key, IPC_CREAT | 0666);
    msg m;
    m.type = 1;
    
    cout << "Sender (type 'exit' to quit)" << endl;
    while(true) {
        cout << "Message: ";
        if(!cin.getline(m.text, 512)) {
            cout << "\nInput error or EOF. Exiting..." << endl;
            break;
        }
        msgsnd(msgid, &m, sizeof(m.text), 0);
        cout << "Sent!" << endl;
        if(strcmp(m.text, "exit") == 0) break;
    }
    
    return 0;
}

