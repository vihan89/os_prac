/*
==============================================================================
ASSIGNMENT 7.1: IPC - MESSAGE QUEUE (SENDER)
==============================================================================

PROBLEM:
- Implement sender program using message queue
- Send messages to receiver process
- Use System V IPC functions: msgget(), msgsnd()

COMPILATION:
g++ 7_1_sender.cpp -o sender

EXECUTION:
1. Run sender: ./sender
2. Run receiver (in another terminal): ./receiver

MESSAGE QUEUE FUNCTIONS:
1. msgget(key, flags): Create/access message queue
2. msgsnd(msgid, msgp, size, flag): Send message
3. msgrcv(msgid, msgp, size, type, flag): Receive message
4. msgctl(msgid, IPC_RMID, NULL): Remove queue

KEY CONCEPTS:
- Key: Unique identifier (use ftok() or IPC_PRIVATE)
- Message type: For selective receiving
- IPC_CREAT: Create if doesn't exist
- 0666: Permissions (read/write for all)
==============================================================================
*/

#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <unistd.h>

using namespace std;

#define MAX_TEXT 512

// Message structure
struct message {
    long msg_type;           // Message type (must be > 0)
    char msg_text[MAX_TEXT]; // Message data
};

int main() {
    key_t key;
    int msgid;
    message msg;
    
    cout << "========================================" << endl;
    cout << "  MESSAGE QUEUE - SENDER" << endl;
    cout << "========================================\n" << endl;
    
    // Generate unique key using ftok()
    // ftok(filepath, project_id) generates key from file path
    key = ftok(".", 'A');  // Use current directory and identifier 'A'
    
    if(key == -1) {
        perror("ftok failed");
        return 1;
    }
    
    cout << "Generated Key: " << key << endl;
    
    // Create message queue
    // IPC_CREAT: Create if doesn't exist
    // 0666: Permissions (rw-rw-rw-)
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
        
        // Check for exit command
        if(strcmp(msg.msg_text, "exit") == 0) {
            cout << "\nExiting sender..." << endl;
            break;
        }
        
        // Set message type (can be used for priority)
        msg.msg_type = 1;  // Type 1 for all messages
        
        // Send message
        // msgsnd(msgid, &msg, size_of_data, flag)
        // size = sizeof(msg) - sizeof(msg_type)
        if(msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
            perror("msgsnd failed");
            continue;
        }
        
        cout << "✓ Message sent successfully!" << endl;
        count++;
        
        sleep(1);  // Small delay
    }
    
    cout << "\n========================================" << endl;
    cout << "Total messages sent: " << count << endl;
    cout << "========================================" << endl;
    
    cout << "\nNote: Message queue remains active." << endl;
    cout << "Run receiver to read messages." << endl;
    cout << "To remove queue, use: ipcrm -q " << msgid << endl;
    
    return 0;
}

/*
==============================================================================
EXAM NOTES:

1. MESSAGE QUEUE BASICS:
   - IPC mechanism for message passing
   - Messages stored in kernel space
   - Processes can send/receive asynchronously
   - Survives process termination
   - Must be explicitly removed

2. SYSTEM V IPC FUNCTIONS:

   ftok(pathname, proj_id):
   - Generates unique key from file path
   - Same path + proj_id → Same key
   - Returns key_t
   
   msgget(key, flags):
   - Creates or accesses message queue
   - Returns message queue ID (msgid)
   - Flags: IPC_CREAT, IPC_EXCL, permissions
   
   msgsnd(msgid, msgp, size, flag):
   - Sends message to queue
   - msgp: Pointer to message structure
   - size: sizeof(msg_text) NOT sizeof(whole struct)
   - flag: 0 (blocking) or IPC_NOWAIT
   
   msgrcv(msgid, msgp, size, type, flag):
   - Receives message from queue
   - type: 0=any, >0=specific type
   - flag: 0 or IPC_NOWAIT or MSG_NOERROR

3. MESSAGE STRUCTURE:
   struct message {
       long msg_type;    // MUST be first, MUST be long
       char msg_text[];  // Your data
   };
   
   - msg_type MUST be > 0
   - Used for selective reception
   - Size in msgsnd/msgrcv excludes msg_type

4. KEY GENERATION METHODS:
   a) ftok("path", 'id'): From file path
   b) IPC_PRIVATE: Private queue (parent-child)
   c) Hardcoded: 1234 (not recommended)

5. FLAGS:
   IPC_CREAT: Create if doesn't exist
   IPC_EXCL: Fail if already exists
   IPC_NOWAIT: Non-blocking send/receive
   0666: Read+Write for user/group/others

6. SEND OPERATION:
   msgsnd(msgid, &msg, sizeof(msg.msg_text), 0)
   
   - Blocking by default
   - If queue full, waits
   - Use IPC_NOWAIT for non-blocking

7. MESSAGE TYPES:
   - For priority/filtering
   - Receiver can select by type
   - Type 1: Normal messages
   - Type 2: High priority
   - Type 0: Receive any type

8. QUEUE MANAGEMENT:
   View queues: ipcs -q
   Remove queue: ipcrm -q <msgid>
   Clear all: ipcrm --all=msg

9. COMMON ERRORS:
   - ftok fails: File doesn't exist
   - msgget fails: Permission denied
   - msgsnd fails: Queue full or invalid msgid
   - Forgetting to remove queue

10. EXAM TIP:
    Must show:
    1. ftok() for key generation
    2. msgget() with IPC_CREAT
    3. Message structure with msg_type
    4. msgsnd() with correct size
    5. Error checking for each call

11. ADVANTAGES:
    - Asynchronous communication
    - Messages preserved if receiver not ready
    - Multiple senders/receivers possible
    - Type-based selection

12. DISADVANTAGES:
    - System limit on queue size
    - Must manually remove queue
    - Not suitable for large data
    - Slower than shared memory

13. SIZE CALCULATION:
    WRONG: sizeof(msg)  // Includes msg_type
    RIGHT: sizeof(msg.msg_text)  // Data only
    
    Or: sizeof(msg) - sizeof(long)

14. WORKFLOW:
    Sender:
    1. ftok() → key
    2. msgget() → msgid
    3. Prepare message with type
    4. msgsnd() in loop
    5. Exit (queue remains)
    
    Receiver:
    1. ftok() with SAME path/id → key
    2. msgget() → msgid (same queue)
    3. msgrcv() in loop
    4. msgctl(IPC_RMID) to remove
==============================================================================
*/
