

#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>

using namespace std;

#define MAX_TEXT 512

struct message {
    long msg_type;
    char msg_text[MAX_TEXT];
};

int main() {
    key_t key;
    int msgid;
    message msg;
    
    cout << "========================================" << endl;
    cout << "  MESSAGE QUEUE - RECEIVER" << endl;
    cout << "========================================\n" << endl;
    

    key = ftok(".", 'A');
    
    if(key == -1) {
        perror("ftok failed");
        return 1;
    }
    
    cout << "Generated Key: " << key << endl;
    

    msgid = msgget(key, 0666);
    
    if(msgid == -1) {
        perror("msgget failed - Queue doesn't exist");
        cout << "\nMake sure sender is running first!" << endl;
        return 1;
    }
    
    cout << "Message Queue ID: " << msgid << endl;
    cout << "Connected to message queue successfully!" << endl;
    
    cout << "\n========================================" << endl;
    cout << "  RECEIVING MESSAGES" << endl;
    cout << "========================================" << endl;
    cout << "Waiting for messages... (Ctrl+C to stop)\n" << endl;
    
    int count = 0;
    
    while(true) {

        
        if(msgrcv(msgid, &msg, sizeof(msg.msg_text), 0, 0) == -1) {
            perror("msgrcv failed");
            break;
        }
        
        count++;
        

        for(int i = 0; msg.msg_text[i] != '\0'; i++) {
            if(msg.msg_text[i] >= 'a' && msg.msg_text[i] <= 'z') {
                msg.msg_text[i] = msg.msg_text[i] - 32;
            }
        }
        
        cout << "Message " << count << " [Type " << msg.msg_type << "]: " 
             << msg.msg_text << " (converted to uppercase)" << endl;
        

        if(strcmp(msg.msg_text, "quit") == 0) {
            cout << "\nReceived quit command. Stopping..." << endl;
            break;
        }
    }
    
    cout << "\n========================================" << endl;
    cout << "Total messages received: " << count << endl;
    cout << "========================================" << endl;
    

    cout << "\nRemoving message queue..." << endl;
    if(msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl failed");
        cout << "Warning: Could not remove message queue" << endl;
        cout << "Remove manually using: ipcrm -q " << msgid << endl;
    }
    else {
        cout << "✓ Message queue removed successfully!" << endl;
    }
    
    return 0;
}

