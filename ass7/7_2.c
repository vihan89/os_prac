/*
7.2 Pipes - String Reversal: 
Write a C program in which the parent process accepts a string from the user and 
writes it into a pipe. The child process reads the string, reverses it, and displays 
the reversed output on the console. Ensure proper closing of pipe ends and error 
handling during communication.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

// Function to reverse a string
void reverseString(char *str) {
    int len = strlen(str);
    for(int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int main() {
    int pipefd[2];
    pid_t pid;
    char write_msg[BUFFER_SIZE];
    char read_msg[BUFFER_SIZE];
    
    printf("========================================\n");
    printf("  PIPE - STRING REVERSAL\n");
    printf("========================================\n\n");
    
    // Create pipe
    if(pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }
    
    printf("Enter a string: ");
    fgets(write_msg, BUFFER_SIZE, stdin);
    write_msg[strcspn(write_msg, "\n")] = '\0';  // Remove newline
    
    pid = fork();
    
    if(pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    
    if(pid > 0) {
        // Parent process
        close(pipefd[0]);  // Close unused read end
        
        printf("\n[Parent] Original string: \"%s\"\n", write_msg);
        printf("[Parent] Writing to pipe...\n");
        
        // Write to pipe
        ssize_t bytes_written = write(pipefd[1], write_msg, strlen(write_msg) + 1);
        
        if(bytes_written == -1) {
            perror("Write failed");
            exit(1);
        }
        
        printf("[Parent] Successfully wrote %ld bytes to pipe\n", bytes_written);
        
        close(pipefd[1]);  // Close write end after writing
        
        // Wait for child to complete
        wait(NULL);
        printf("\n[Parent] Child process completed\n");
        
    } else {
        // Child process
        close(pipefd[1]);  // Close unused write end
        
        printf("[Child] Reading from pipe...\n");
        
        // Read from pipe
        ssize_t bytes_read = read(pipefd[0], read_msg, BUFFER_SIZE);
        
        if(bytes_read == -1) {
            perror("Read failed");
            exit(1);
        }
        
        printf("[Child] Successfully read %ld bytes from pipe\n", bytes_read);
        printf("[Child] Received string: \"%s\"\n", read_msg);
        
        close(pipefd[0]);  // Close read end after reading
        
        // Reverse the string
        printf("[Child] Reversing string...\n");
        reverseString(read_msg);
        
        printf("\n========================================\n");
        printf("  REVERSED OUTPUT\n");
        printf("========================================\n");
        printf("%s\n", read_msg);
        printf("========================================\n");
        
        exit(0);
    }
    
    return 0;
}
