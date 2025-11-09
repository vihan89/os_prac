#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

bool isPrime(int n) {
    if(n < 2) return false;
    for(int i = 2; i * i <= n; i++) {
        if(n % i == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Usage: ./program <number_of_primes>\n");
        return 1;
    }
    
    int n = atoi(argv[1]);
    
    if(n <= 0) {
        printf("Error: Please provide a positive integer\n");
        return 1;
    }
    
    pid_t pid = fork();
    
    if(pid == 0) {
        printf("Child Process: Generating %d prime numbers\n", n);
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
    } 
    else {
        wait(NULL);
        printf("Parent Process: Child has finished execution\n");
    }
    
    return 0;
}
