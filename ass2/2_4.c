/*
2.4 Write a C program using the fork() system call that generates the sequence in the child process. 
The starting number will be provided from the command line. 
For example, if 8 is passed as a parameter on the command line, the child process will output 8, 4, 2, 1. 
i.e., when we take any positive integer n and apply the following algorithm, 
it will eventually reach 1:
    n = n/2, if n is even
    n = 3 × n + 1, if n is odd

Because the parent and child processes have their own copies of the data, 
it will be necessary for the child to output the sequence. 
Have the parent invoke the wait() call to wait for the child process to complete before exiting the program. 
Perform necessary error checking to ensure that a positive integer is passed on the command line.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Error checking for command line arguments
    if(argc != 2) {
        printf("Usage: %s <positive_integer>\n", argv[0]);
        return 1;
    }
    
    int n = atoi(argv[1]);
    
    // Check if positive integer
    if(n <= 0) {
        printf("Error: Please provide a positive integer!\n");
        return 1;
    }
    
    printf("Starting number: %d\n\n", n);
    
    pid_t pid = fork();
    
    if(pid < 0) {
        printf("Fork failed!\n");
        return 1;
    }
    else if(pid == 0) {
        // Child process - Generate sequence
        printf("Child Process (PID: %d)\n", getpid());
        printf("Collatz Sequence: ");
        
        while(n != 1) {
            printf("%d ", n);
            
            if(n % 2 == 0) {
                // n is even
                n = n / 2;
            }
            else {
                // n is odd
                n = 3 * n + 1;
            }
        }
        
        printf("%d\n", n); // Print final 1
        
        printf("Child process exiting...\n");
        exit(0);
    }
    else {
        // Parent process
        printf("Parent Process (PID: %d)\n", getpid());
        printf("Parent waiting for child to complete...\n\n");
        
        // Wait for child to complete
        wait(NULL);
        
        printf("\nParent: Child process has completed.\n");
        printf("Parent exiting...\n");
    }
    
    return 0;
}
