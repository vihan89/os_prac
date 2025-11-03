/*
5.2 Develop a C program to simulate Banker's Algorithm for Deadlock Avoidance 
with following requirements:
- Demonstrate safe and unsafe state of the system
- Demonstrate Grant and non-grant of new resource request
Note: Your program may read the current state of the system from a file called "state.txt".
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

// Function to read state from file
bool readStateFromFile(const char* filename) {
    FILE *fp = fopen(filename, "r");
    
    if(fp == NULL) {
        printf("Error: Cannot open file '%s'\n", filename);
        return false;
    }
    
    fscanf(fp, "%d %d", &n, &m);
    
    // Read Allocation matrix
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            fscanf(fp, "%d", &allocation[i][j]);
        }
    }
    
    // Read Max matrix
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            fscanf(fp, "%d", &max[i][j]);
        }
    }
    
    // Read Available resources
    for(int i = 0; i < m; i++) {
        fscanf(fp, "%d", &available[i]);
    }
    
    fclose(fp);
    return true;
}

// Function to display current state
void displayState() {
    printf("\n===== Current System State =====\n");
    
    printf("\nAllocation Matrix:\n");
    printf("Process\t");
    for(int j = 0; j < m; j++) printf("R%d\t", j);
    printf("\n");
    for(int i = 0; i < n; i++) {
        printf("P%d\t", i);
        for(int j = 0; j < m; j++) printf("%d\t", allocation[i][j]);
        printf("\n");
    }
    
    printf("\nMax Matrix:\n");
    printf("Process\t");
    for(int j = 0; j < m; j++) printf("R%d\t", j);
    printf("\n");
    for(int i = 0; i < n; i++) {
        printf("P%d\t", i);
        for(int j = 0; j < m; j++) printf("%d\t", max[i][j]);
        printf("\n");
    }
    
    printf("\nNeed Matrix:\n");
    printf("Process\t");
    for(int j = 0; j < m; j++) printf("R%d\t", j);
    printf("\n");
    for(int i = 0; i < n; i++) {
        printf("P%d\t", i);
        for(int j = 0; j < m; j++) printf("%d\t", need[i][j]);
        printf("\n");
    }
    
    printf("\nAvailable Resources: ");
    for(int j = 0; j < m; j++) printf("R%d:%d  ", j, available[j]);
    printf("\n");
}

// Function to check if request can be granted
bool requestResources(int process_id, int request[]) {
    printf("\n========================================\n");
    printf("Processing Request from P%d\n", process_id);
    printf("========================================\n");
    
    printf("Requested: ");
    for(int i = 0; i < m; i++) {
        printf("R%d:%d  ", i, request[i]);
    }
    printf("\n");
    
    // Check if request <= need
    for(int i = 0; i < m; i++) {
        if(request[i] > need[process_id][i]) {
            printf("\nREJECTED: Request exceeds declared need!\n");
            return false;
        }
    }
    
    // Check if request <= available
    for(int i = 0; i < m; i++) {
        if(request[i] > available[i]) {
            printf("\nREJECTED: Insufficient resources available!\n");
            printf("Process must wait.\n");
            return false;
        }
    }
    
    // Temporarily allocate resources
    int temp_available[MAX_RESOURCES];
    int temp_allocation[MAX_RESOURCES];
    int temp_need[MAX_RESOURCES];
    
    for(int i = 0; i < m; i++) {
        temp_available[i] = available[i];
        temp_allocation[i] = allocation[process_id][i];
        temp_need[i] = need[process_id][i];
        
        available[i] -= request[i];
        allocation[process_id][i] += request[i];
        need[process_id][i] -= request[i];
    }
    
    // Check if resulting state is safe
    int safe_sequence[MAX_PROCESSES];
    bool is_safe = isSafe(safe_sequence);
    
    if(is_safe) {
        printf("\nGRANTED: Request can be granted safely!\n");
        printf("New state is SAFE.\n");
        printf("Safe Sequence: < ");
        for(int i = 0; i < n; i++) {
            printf("P%d", safe_sequence[i]);
            if(i < n - 1) printf(", ");
        }
        printf(" >\n");
        return true;
    } else {
        // Rollback
        for(int i = 0; i < m; i++) {
            available[i] = temp_available[i];
            allocation[process_id][i] = temp_allocation[i];
            need[process_id][i] = temp_need[i];
        }
        
        printf("\nREJECTED: Granting would lead to UNSAFE state!\n");
        printf("Request denied to avoid potential deadlock.\n");
        return false;
    }
}

// Function to create sample state.txt file
void createSampleFile() {
    FILE *fp = fopen("state.txt", "w");
    if(fp == NULL) return;
    
    fprintf(fp, "5 3\n");  // 5 processes, 3 resource types
    
    // Allocation matrix
    fprintf(fp, "0 1 0\n");
    fprintf(fp, "2 0 0\n");
    fprintf(fp, "3 0 2\n");
    fprintf(fp, "2 1 1\n");
    fprintf(fp, "0 0 2\n");
    
    // Max matrix
    fprintf(fp, "7 5 3\n");
    fprintf(fp, "3 2 2\n");
    fprintf(fp, "9 0 2\n");
    fprintf(fp, "2 2 2\n");
    fprintf(fp, "4 3 3\n");
    
    // Available resources
    fprintf(fp, "3 3 2\n");
    
    fclose(fp);
    printf("Sample 'state.txt' file created!\n\n");
}

int main() {
    int choice, process_id;
    int request[MAX_RESOURCES];
    int safe_sequence[MAX_PROCESSES];
    
    printf("========================================\n");
    printf("   BANKER'S ALGORITHM - DEADLOCK AVOIDANCE\n");
    printf("========================================\n\n");
    
    // Try to read from file, if not exists create sample
    if(!readStateFromFile("state.txt")) {
        printf("Creating sample state.txt file...\n");
        createSampleFile();
        readStateFromFile("state.txt");
    }
    
    printf("Successfully loaded system state from 'state.txt'\n");
    printf("Processes: %d, Resource Types: %d\n", n, m);
    
    // Calculate need matrix
    calculateNeed();
    
    while(1) {
        printf("\n========================================\n");
        printf("              MENU\n");
        printf("========================================\n");
        printf("1. Display Current State\n");
        printf("2. Check System Safety\n");
        printf("3. Request Resources\n");
        printf("4. Exit\n");
        printf("========================================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                displayState();
                break;
                
            case 2:
                printf("\n========================================\n");
                printf("       SAFETY ALGORITHM\n");
                printf("========================================\n");
                
                if(isSafe(safe_sequence)) {
                    printf("\nSystem is in SAFE state!\n");
                    printf("Safe Sequence: < ");
                    for(int i = 0; i < n; i++) {
                        printf("P%d", safe_sequence[i]);
                        if(i < n - 1) printf(", ");
                    }
                    printf(" >\n");
                } else {
                    printf("\nSystem is in UNSAFE state!\n");
                    printf("Deadlock may occur!\n");
                }
                break;
                
            case 3:
                printf("\nEnter process number (0-%d): ", n-1);
                scanf("%d", &process_id);
                
                if(process_id < 0 || process_id >= n) {
                    printf("Invalid process number!\n");
                    break;
                }
                
                printf("Enter resource request (%d values): ", m);
                for(int i = 0; i < m; i++) {
                    scanf("%d", &request[i]);
                }
                
                requestResources(process_id, request);
                break;
                
            case 4:
                printf("\nExiting...\n");
                exit(0);
                
            default:
                printf("\nInvalid choice!\n");
        }
    }
    
    return 0;
}
