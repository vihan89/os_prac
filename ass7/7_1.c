/*
7.1 Pipes - Case Reversal: 
Design a program using ordinary pipes in which one process sends a string message 
to a second process, and the second process reverses the case of each character in 
the message and sends it back to the first process. For example, if the first process 
sends the message "Hi There", the second process will return "hI tHERE". 
This will require using two pipes, one for sending the original message from the 
first to the second process and the other for sending the modified message from 
the second to the first process.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

#define BUFFER_SIZE 256

// Function to reverse case of each character
void reverseCase(char *str) {
    for(int i = 0; str[i] != '\0'; i++) {
        if(isupper(str[i])) {
            str[i] = tolower(str[i]);
        } else if(islower(str[i])) {
            str[i] = toupper(str[i]);
        }
    }
}

int main() {
    int pipe1[2];  // Pipe for parent to child
    int pipe2[2];  // Pipe for child to parent
    pid_t pid;
    char write_msg[BUFFER_SIZE];
    char read_msg[BUFFER_SIZE];
    
    printf("========================================\n");
    printf("  PIPE - CASE REVERSAL\n");
    printf("========================================\n\n");
    
    // Create first pipe (parent -> child)
    if(pipe(pipe1) == -1) {
        perror("Pipe1 creation failed");
        exit(1);
    }
    
    // Create second pipe (child -> parent)
    if(pipe(pipe2) == -1) {
        perror("Pipe2 creation failed");
        exit(1);
    }
    
    printf("Enter a message: ");
    fgets(write_msg, BUFFER_SIZE, stdin);
    write_msg[strcspn(write_msg, "\n")] = '\0';  // Remove newline
    
    pid = fork();
    
    if(pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    
    if(pid > 0) {
        // Parent process
        close(pipe1[0]);  // Close unused read end of pipe1
        close(pipe2[1]);  // Close unused write end of pipe2
        
        printf("\n[Parent] Original message: \"%s\"\n", write_msg);
        printf("[Parent] Sending message to child...\n");
        
        // Write to pipe1 (send to child)
        write(pipe1[1], write_msg, strlen(write_msg) + 1);
        close(pipe1[1]);  // Close write end after writing
        
        // Read from pipe2 (receive from child)
        read(pipe2[0], read_msg, BUFFER_SIZE);
        close(pipe2[0]);  // Close read end after reading
        
        printf("[Parent] Received from child: \"%s\"\n", read_msg);
        
        wait(NULL);  // Wait for child to complete
        
        printf("\n========================================\n");
        printf("  RESULT\n");
        printf("========================================\n");
        printf("Original:  %s\n", write_msg);
        printf("Modified:  %s\n", read_msg);
        printf("========================================\n");
        
    } else {
        // Child process
        close(pipe1[1]);  // Close unused write end of pipe1
        close(pipe2[0]);  // Close unused read end of pipe2
        
        // Read from pipe1 (receive from parent)
        read(pipe1[0], read_msg, BUFFER_SIZE);
        close(pipe1[0]);  // Close read end after reading
        
        printf("[Child] Received message: \"%s\"\n", read_msg);
        printf("[Child] Reversing case...\n");
        
        // Reverse the case
        reverseCase(read_msg);
        
        printf("[Child] Sending back: \"%s\"\n", read_msg);
        
        // Write to pipe2 (send to parent)
        write(pipe2[1], read_msg, strlen(read_msg) + 1);
        close(pipe2[1]);  // Close write end after writing
        
        exit(0);
    }
    
    return 0;
}
