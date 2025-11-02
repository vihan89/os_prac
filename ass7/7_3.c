/*
7.3 Message Queues: 
Implement two independent processes, Sender and Receiver, using Message Queues (FIFO). 
The Sender accepts messages from the user and sends them to the queue with different 
message types. The Receiver reads each message, converts it to uppercase, and displays 
it on the screen. Ensure proper cleanup of message queues after execution.

Compile and run:
gcc 7_3.c -o 7_3
./7_3 sender    # Run in one terminal
./7_3 receiver  # Run in another terminal
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_TEXT 512
#define MSG_KEY 1234

// Message structure
struct message {
    long msg_type;
    char msg_text[MAX_TEXT];
};

// Function to convert string to uppercase
void toUpperCase(char *str) {
    for(int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

// Sender process
void sender() {
    int msgid;
    struct message msg;
    
    printf("========================================\n");
    printf("  MESSAGE QUEUE - SENDER\n");
    printf("========================================\n\n");
    
    // Create or get message queue
    msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if(msgid == -1) {
        perror("msgget failed");
        exit(1);
    }
    
    printf("Message Queue created with ID: %d\n\n", msgid);
    
    while(1) {
        printf("Enter message type (1-5, 0 to quit): ");
        scanf("%ld", &msg.msg_type);
        getchar();  // Consume newline
        
        if(msg.msg_type == 0) {
            printf("\nExiting sender...\n");
            break;
        }
        
        if(msg.msg_type < 1 || msg.msg_type > 5) {
            printf("Invalid message type! Use 1-5\n");
            continue;
        }
        
        printf("Enter message text: ");
        fgets(msg.msg_text, MAX_TEXT, stdin);
        msg.msg_text[strcspn(msg.msg_text, "\n")] = '\0';
        
        // Send message to queue
        if(msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
            perror("msgsnd failed");
            exit(1);
        }
        
        printf("[Sent] Type: %ld, Message: \"%s\"\n\n", msg.msg_type, msg.msg_text);
    }
}

// Receiver process
void receiver() {
    int msgid;
    struct message msg;
    
    printf("========================================\n");
    printf("  MESSAGE QUEUE - RECEIVER\n");
    printf("========================================\n\n");
    
    // Get existing message queue
    msgid = msgget(MSG_KEY, 0666);
    if(msgid == -1) {
        perror("msgget failed - Queue doesn't exist. Start sender first!");
        exit(1);
    }
    
    printf("Connected to Message Queue ID: %d\n", msgid);
    printf("Waiting for messages...\n");
    printf("(Press Ctrl+C to stop)\n\n");
    
    while(1) {
        // Receive message from queue (0 means receive any type)
        if(msgrcv(msgid, &msg, sizeof(msg.msg_text), 0, 0) == -1) {
            perror("msgrcv failed");
            break;
        }
        
        printf("----------------------------------------\n");
        printf("[Received] Type: %ld\n", msg.msg_type);
        printf("[Original] %s\n", msg.msg_text);
        
        // Convert to uppercase
        toUpperCase(msg.msg_text);
        
        printf("[Uppercase] %s\n", msg.msg_text);
        printf("----------------------------------------\n\n");
    }
    
    // Cleanup: Remove message queue
    printf("\nCleaning up message queue...\n");
    if(msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl failed");
        exit(1);
    }
    printf("Message queue removed successfully.\n");
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s [sender|receiver]\n", argv[0]);
        printf("\nExamples:\n");
        printf("  %s sender    - Run as sender process\n", argv[0]);
        printf("  %s receiver  - Run as receiver process\n", argv[0]);
        printf("\nNote: Run sender in one terminal, receiver in another\n");
        return 1;
    }
    
    if(strcmp(argv[1], "sender") == 0) {
        sender();
    } else if(strcmp(argv[1], "receiver") == 0) {
        receiver();
    } else {
        printf("Invalid argument! Use 'sender' or 'receiver'\n");
        return 1;
    }
    
    return 0;
}
