

#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <unistd.h>

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
    cout << "  MESSAGE QUEUE - SENDER" << endl;
    cout << "========================================\n" << endl;
    

    key = ftok(".", 'A');
    
    if(key == -1) {
        perror("ftok failed");
        return 1;
    }
    
    cout << "Generated Key: " << key << endl;
    

    msgid = msgget(key, IPC_CREAT | 0666);
    
    if(msgid == -1) {
        perror("msgget failed");
        return 1;
    }
    
    cout << "Message Queue ID: " << msgid << endl;
    cout << "Message queue created successfully!" << endl;
    
    cout << "\n========================================" << endl;
    cout << "  SENDING MESSAGES" << endl;
    cout << "========================================" << endl;
    cout << "Enter messages to send (type 'exit' to quit)" << endl;
    cout << "Format: Type a message and press Enter\n" << endl;
    
    int count = 0;
    
    while(true) {
        cout << "Message " << (count + 1) << ": ";
        cin.getline(msg.msg_text, MAX_TEXT);
        

        if(strcmp(msg.msg_text, "exit") == 0) {
            cout << "\nExiting sender..." << endl;
            break;
        }
        

        msg.msg_type = 1;
        

        if(msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
            perror("msgsnd failed");
            continue;
        }
        
        cout << "✓ Message sent successfully!" << endl;
        count++;
        
        sleep(1);
    }
    
    cout << "\n========================================" << endl;
    cout << "Total messages sent: " << count << endl;
    cout << "========================================" << endl;
    
    cout << "\nNote: Message queue remains active." << endl;
    cout << "Run receiver to read messages." << endl;
    cout << "To remove queue, use: ipcrm -q " << msgid << endl;
    
    return 0;
}

