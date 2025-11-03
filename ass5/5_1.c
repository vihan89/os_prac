/*
5.1 Develop a C program to implement Banker's algorithm. 
Assume suitable input required to demonstrate the results. 
Using the banker's algorithm, determine whether or not the state is unsafe. 
If the state is safe, illustrate the order in which the processes may complete. 
Otherwise, illustrate why the state is unsafe.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int n; // Number of processes
int m; // Number of resource types

int allocation[MAX_PROCESSES][MAX_RESOURCES];
int max[MAX_PROCESSES][MAX_RESOURCES];
int available[MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];

// Function to calculate need matrix
void calculateNeed() {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

// Function to check if system is in safe state
bool isSafe(int safe_sequence[]) {
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};
    
    // Initialize work with available resources
    for(int i = 0; i < m; i++) {
        work[i] = available[i];
    }
    
    int count = 0;
    
    while(count < n) {
        bool found = false;
        
        for(int i = 0; i < n; i++) {
            if(!finish[i]) {
                bool can_allocate = true;
                
                // Check if need <= work
                for(int j = 0; j < m; j++) {
                    if(need[i][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }
                
                if(can_allocate) {
                    // Add allocated resources back to work
                    for(int j = 0; j < m; j++) {
                        work[j] += allocation[i][j];
                    }
                    
                    safe_sequence[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }
        
        if(!found) {
            return false; // System is in unsafe state
        }
    }
    
    return true; // System is in safe state
}

// Function to display matrices
void displayMatrices() {
    printf("\n===== Allocation Matrix =====\n");
    printf("Process\t");
    for(int j = 0; j < m; j++) {
        printf("R%d\t", j);
    }
    printf("\n");
    
    for(int i = 0; i < n; i++) {
        printf("P%d\t", i);
        for(int j = 0; j < m; j++) {
            printf("%d\t", allocation[i][j]);
        }
        printf("\n");
    }
    
    printf("\n===== Max Matrix =====\n");
    printf("Process\t");
    for(int j = 0; j < m; j++) {
        printf("R%d\t", j);
    }
    printf("\n");
    
    for(int i = 0; i < n; i++) {
        printf("P%d\t", i);
        for(int j = 0; j < m; j++) {
            printf("%d\t", max[i][j]);
        }
        printf("\n");
    }
    
    printf("\n===== Need Matrix =====\n");
    printf("Process\t");
    for(int j = 0; j < m; j++) {
        printf("R%d\t", j);
    }
    printf("\n");
    
    for(int i = 0; i < n; i++) {
        printf("P%d\t", i);
        for(int j = 0; j < m; j++) {
            printf("%d\t", need[i][j]);
        }
        printf("\n");
    }
    
    printf("\n===== Available Resources =====\n");
    for(int j = 0; j < m; j++) {
        printf("R%d: %d\t", j, available[j]);
    }
    printf("\n");
}

int main() {
    int safe_sequence[MAX_PROCESSES];
    
    printf("========================================\n");
    printf("    BANKER'S ALGORITHM IMPLEMENTATION\n");
    printf("========================================\n\n");
    
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    printf("Enter number of resource types: ");
    scanf("%d", &m);
    
    printf("\nEnter Allocation Matrix (%d x %d):\n", n, m);
    for(int i = 0; i < n; i++) {
        printf("Process P%d: ", i);
        for(int j = 0; j < m; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }
    
    printf("\nEnter Max Matrix (%d x %d):\n", n, m);
    for(int i = 0; i < n; i++) {
        printf("Process P%d: ", i);
        for(int j = 0; j < m; j++) {
            scanf("%d", &max[i][j]);
        }
    }
    
    printf("\nEnter Available Resources (%d resources):\n", m);
    for(int i = 0; i < m; i++) {
        scanf("%d", &available[i]);
    }
    
    // Calculate need matrix
    calculateNeed();
    
    // Display all matrices
    displayMatrices();
    
    // Check for safe state
    printf("\n========================================\n");
    printf("       CHECKING SYSTEM STATE\n");
    printf("========================================\n");
    
    if(isSafe(safe_sequence)) {
        printf("\nThe system is in SAFE state!\n");
        printf("\nSafe Sequence: < ");
        for(int i = 0; i < n; i++) {
            printf("P%d", safe_sequence[i]);
            if(i < n - 1) printf(", ");
        }
        printf(" >\n");
        
        printf("\nProcesses can complete in the following order:\n");
        for(int i = 0; i < n; i++) {
            printf("Step %d: Process P%d can complete\n", i + 1, safe_sequence[i]);
        }
    } else {
        printf("\nThe system is in UNSAFE state!\n");
        printf("\nReason: No safe sequence exists.\n");
        printf("The system cannot guarantee that all processes will complete.\n");
        printf("Deadlock may occur!\n");
    }
    
    printf("\n========================================\n");
    
    return 0;
}
