/*
3.3 Write a menu-driven C program to simulate the following CPU scheduling algorithms 
to find average turnaround time and average waiting time.
a) FCFS
b) Round-robin (preemptive) with time quantum = 2 sec.
*/

#include <stdio.h>
#include <stdlib.h>

// Structure to represent a process
typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

// Function to display process details
void displayProcesses(Process p[], int n) {
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("-------------------------------------------\n");
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", 
               p[i].pid, p[i].arrival_time, p[i].burst_time,
               p[i].completion_time, p[i].turnaround_time, p[i].waiting_time);
    }
}

// Function to calculate average times
void calculateAverage(Process p[], int n) {
    float total_tat = 0, total_wt = 0;
    
    for(int i = 0; i < n; i++) {
        total_tat += p[i].turnaround_time;
        total_wt += p[i].waiting_time;
    }
    
    printf("\nAverage Turnaround Time: %.2f", total_tat / n);
    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
}

// FCFS Scheduling Algorithm
void fcfs(Process p[], int n) {
    int current_time = 0;
    
    // Sort by arrival time
    for(int i = 0; i < n - 1; i++) {
        for(int j = 0; j < n - i - 1; j++) {
            if(p[j].arrival_time > p[j + 1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
    
    for(int i = 0; i < n; i++) {
        if(current_time < p[i].arrival_time) {
            current_time = p[i].arrival_time;
        }
        
        p[i].completion_time = current_time + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
        
        current_time = p[i].completion_time;
    }
    
    printf("\n=== FCFS Scheduling ===\n");
    displayProcesses(p, n);
    calculateAverage(p, n);
}

// Round Robin Scheduling Algorithm
void round_robin(Process p[], int n, int quantum) {
    int current_time = 0;
    int completed = 0;
    int queue[100], front = 0, rear = 0;
    int visited[n];
    
    // Initialize
    for(int i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
        visited[i] = 0;
    }
    
    // Sort by arrival time
    for(int i = 0; i < n - 1; i++) {
        for(int j = 0; j < n - i - 1; j++) {
            if(p[j].arrival_time > p[j + 1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
    
    // Add first process to queue
    queue[rear++] = 0;
    visited[0] = 1;
    
    while(completed != n) {
        if(front == rear) {
            // Queue is empty, find next arrived process
            for(int i = 0; i < n; i++) {
                if(p[i].remaining_time > 0 && !visited[i] && p[i].arrival_time <= current_time) {
                    queue[rear++] = i;
                    visited[i] = 1;
                    break;
                }
            }
            if(front == rear) {
                current_time++;
                continue;
            }
        }
        
        int idx = queue[front++];
        
        if(p[idx].remaining_time <= quantum) {
            // Process will complete
            current_time += p[idx].remaining_time;
            p[idx].remaining_time = 0;
            p[idx].completion_time = current_time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
            completed++;
        }
        else {
            // Process will not complete in this quantum
            current_time += quantum;
            p[idx].remaining_time -= quantum;
        }
        
        // Add newly arrived processes to queue
        for(int i = 0; i < n; i++) {
            if(p[i].remaining_time > 0 && !visited[i] && p[i].arrival_time <= current_time) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }
        
        // Add current process back to queue if not completed
        if(p[idx].remaining_time > 0) {
            queue[rear++] = idx;
        }
    }
    
    printf("\n=== Round Robin Scheduling (Time Quantum = %d) ===\n", quantum);
    displayProcesses(p, n);
    calculateAverage(p, n);
}

int main() {
    int n, choice;
    int time_quantum = 2;
    
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    Process processes[n], processes_copy[n];
    
    for(int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("\nProcess %d:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burst_time);
    }
    
    while(1) {
        // Copy original data for each algorithm
        for(int i = 0; i < n; i++) {
            processes_copy[i] = processes[i];
        }
        
        printf("\n\n========== MENU ==========\n");
        printf("1. FCFS Scheduling\n");
        printf("2. Round Robin Scheduling (TQ = 2)\n");
        printf("3. Exit\n");
        printf("==========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                fcfs(processes_copy, n);
                break;
            case 2:
                round_robin(processes_copy, n, time_quantum);
                break;
            case 3:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
    
    return 0;
}
