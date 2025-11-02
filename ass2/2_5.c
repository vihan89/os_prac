/*
2.5 Write a C program using the fork() system call that generates the prime number sequence 
in the child process. The number will be provided from the command line. 
For example, if 10 is passed as a parameter on the command line, 
the child process will output 2, 3, 5, 7, 11, 13, 17, 19, 23, 29. 

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

// Function to check if a number is prime
int isPrime(int num) {
    if(num <= 1)
        return 0;
    if(num == 2)
        return 1;
    if(num % 2 == 0)
        return 0;
    
    for(int i = 3; i * i <= num; i += 2) {
        if(num % i == 0)
            return 0;
    }
    
    return 1;
}

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
    
    printf("Number of primes to generate: %d\n\n", n);
    
    pid_t pid = fork();
    
    if(pid < 0) {
        printf("Fork failed!\n");
        return 1;
    }
    else if(pid == 0) {
        // Child process - Generate prime sequence
        printf("Child Process (PID: %d)\n", getpid());
        printf("First %d prime numbers: ", n);
        
        int count = 0;
        int num = 2;
        
        while(count < n) {
            if(isPrime(num)) {
                printf("%d ", num);
                count++;
            }
            num++;
        }
        
        printf("\n");
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
