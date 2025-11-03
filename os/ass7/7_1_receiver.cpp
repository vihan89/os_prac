/*
==============================================================================
ASSIGNMENT 7.1: IPC - MESSAGE QUEUE (RECEIVER)
==============================================================================

PROBLEM:
- Implement receiver program using message queue
- Receive messages from sender process
- Use System V IPC functions: msgget(), msgrcv(), msgctl()

COMPILATION:
g++ 7_1_receiver.cpp -o receiver

EXECUTION:
1. Run sender first: ./sender
2. Run receiver: ./receiver

NOTE:
- Sender and receiver must use SAME key
- Receiver removes queue after reading
- Can have multiple receivers
==============================================================================
*/

#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>

using namespace std;

#define MAX_TEXT 512

// Message structure (MUST match sender's structure)
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
    
    // Generate SAME key as sender
    key = ftok(".", 'A');  // Same path and identifier
    
    if(key == -1) {
        perror("ftok failed");
        return 1;
    }
    
    cout << "Generated Key: " << key << endl;
    
    // Access existing message queue
    // Don't use IPC_CREAT here if queue should exist
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
        // Receive message
        // msgrcv(msgid, &msg, size, type, flag)
        // type=0: Receive any message type
        // type>0: Receive specific type
        // flag=0: Blocking (wait for message)
        
        if(msgrcv(msgid, &msg, sizeof(msg.msg_text), 0, 0) == -1) {
            perror("msgrcv failed");
            break;
        }
        
        count++;
        
        // Convert message to uppercase
        for(int i = 0; msg.msg_text[i] != '\0'; i++) {
            if(msg.msg_text[i] >= 'a' && msg.msg_text[i] <= 'z') {
                msg.msg_text[i] = msg.msg_text[i] - 32;  // Convert to uppercase
            }
        }
        
        cout << "Message " << count << " [Type " << msg.msg_type << "]: " 
             << msg.msg_text << " (converted to uppercase)" << endl;
        
        // Optional: Exit on specific message
        if(strcmp(msg.msg_text, "quit") == 0) {
            cout << "\nReceived quit command. Stopping..." << endl;
            break;
        }
    }
    
    cout << "\n========================================" << endl;
    cout << "Total messages received: " << count << endl;
    cout << "========================================" << endl;
    
    // Remove message queue
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

/*
==============================================================================
EXAM NOTES:

1. RECEIVER RESPONSIBILITIES:
   - Access existing queue (same key as sender)
   - Receive messages using msgrcv()
   - Process messages
   - Remove queue using msgctl() when done

2. msgrcv() DETAILS:
   ssize_t msgrcv(int msgid, void *msgp, size_t msgsz, 
                  long msgtyp, int msgflg);
   
   Parameters:
   - msgid: Queue identifier from msgget()
   - msgp: Pointer to message structure
   - msgsz: Size of message data (NOT including msg_type)
   - msgtyp: Message type to receive
   - msgflg: Flags (0, IPC_NOWAIT, MSG_NOERROR)
   
   Returns:
   - Number of bytes received on success
   - -1 on error

3. MESSAGE TYPE (msgtyp parameter):
   msgtyp = 0: Receive first message of any type
   msgtyp > 0: Receive first message of type msgtyp
   msgtyp < 0: Receive first message with lowest type <= |msgtyp|
   
   Examples:
   msgrcv(..., 0, 0): Any message
   msgrcv(..., 1, 0): Only type 1
   msgrcv(..., -5, 0): Lowest type ≤ 5

4. FLAGS (msgflg):
   0: Blocking receive (wait for message)
   IPC_NOWAIT: Non-blocking (return immediately if no message)
   MSG_NOERROR: Truncate message if too large
   
   Example:
   msgrcv(msgid, &msg, size, 0, IPC_NOWAIT);
   // Returns -1 with errno=ENOMSG if no message

5. msgctl() - CONTROL OPERATIONS:
   int msgctl(int msgid, int cmd, struct msqid_ds *buf);
   
   Commands:
   - IPC_STAT: Get queue status
   - IPC_SET: Set queue attributes
   - IPC_RMID: Remove queue (most important)
   
   Remove queue:
   msgctl(msgid, IPC_RMID, NULL);

6. SYNCHRONIZATION:
   - Receiver blocks on msgrcv() if no message
   - Queue acts as buffer
   - Messages preserved even if receiver not running
   - Order preserved (FIFO within same type)

7. MULTIPLE RECEIVERS:
   - Multiple processes can receive from same queue
   - Each message received by only ONE receiver
   - First receiver gets message (removed from queue)
   - Use different message types for routing

8. ERROR HANDLING:
   errno values:
   - EACCES: Permission denied
   - EIDRM: Queue removed
   - EINVAL: Invalid msgid or size
   - ENOMSG: No message (with IPC_NOWAIT)

9. QUEUE LIMITS:
   - System limit on queue size (check: ipcs -ql)
   - MSGMAX: Max message size (~8KB)
   - MSGMNB: Max queue size (~16KB)
   - MSGMNI: Max queues system-wide

10. CLEANUP:
    WHY remove queue?
    - Persists after process exits
    - Consumes system resources
    - May fill up over time
    
    When to remove?
    - Receiver done reading
    - Application shutdown
    - Or manually: ipcrm -q <msgid>

11. EXAM WORKFLOW:
    1. ftok() with SAME parameters as sender
    2. msgget() to access queue (no IPC_CREAT)
    3. Loop msgrcv() to receive messages
    4. Process each message
    5. msgctl(IPC_RMID) to cleanup

12. EXAMPLE CODE FLOW:
    key = ftok(".", 'A');
    msgid = msgget(key, 0666);
    
    while(1) {
        msgrcv(msgid, &msg, sizeof(msg.msg_text), 0, 0);
        printf("Received: %s\n", msg.msg_text);
    }
    
    msgctl(msgid, IPC_RMID, NULL);

13. PRIORITY MESSAGES:
    Sender:
    msg.msg_type = 1;  // Low priority
    msg.msg_type = 2;  // High priority
    
    Receiver:
    msgrcv(msgid, &msg, size, 2, 0);  // Get high priority first
    msgrcv(msgid, &msg, size, 0, 0);  // Then any

14. EXAM TIP:
    Always show:
    1. Same ftok() as sender
    2. msgrcv() in loop
    3. Type parameter usage
    4. msgctl() to remove queue
    5. Error checking

15. COMMON MISTAKES:
    - Different key than sender
    - Wrong message type filter
    - Not removing queue
    - Incorrect size parameter
    - Missing error handling

16. DEBUGGING:
    View queues: ipcs -q
    Queue details: ipcs -qi <msgid>
    Remove: ipcrm -q <msgid>
    Remove all: ipcrm --all=msg
==============================================================================
*/
