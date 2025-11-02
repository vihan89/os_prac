// Assignment 2 - Problem 3: Dining Philosophers Problem
#include <stdio.h>

int main() {
    int n = 5; // Number of philosophers
    int fork[5] = {1, 1, 1, 1, 1}; // All forks available
    int choice, phil;
    
    printf("Dining Philosophers Problem\n");
    
    while(1) {
        printf("\n1. Pick up forks\n2. Put down forks\n3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        if(choice == 3) break;
        
        printf("Enter philosopher number (0-4): ");
        scanf("%d", &phil);
        
        if(phil < 0 || phil >= n) {
            printf("Invalid philosopher number!\n");
            continue;
        }
        
        if(choice == 1) {
            // Pick up forks
            if(fork[phil] == 1 && fork[(phil+1) % n] == 1) {
                fork[phil] = 0;
                fork[(phil+1) % n] = 0;
                printf("Philosopher %d is eating\n", phil);
            } else {
                printf("Forks not available for philosopher %d\n", phil);
            }
        } else if(choice == 2) {
            // Put down forks
            fork[phil] = 1;
            fork[(phil+1) % n] = 1;
            printf("Philosopher %d finished eating\n", phil);
        }
    }
    
    return 0;
}
