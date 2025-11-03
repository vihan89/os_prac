/*
3.1 Write a menu-driven C program to simulate the following CPU scheduling algorithms 
to find average turnaround time and average waiting time.
a) FCFS
b) SJF (non-preemptive)
*/

#include <stdio.h>
#include <stdlib.h>

// Structure to represent a process
typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
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

// SJF Non-Preemptive Scheduling Algorithm
void sjf_non_preemptive(Process p[], int n) {
    int completed = 0, current_time = 0;
    int is_completed[n];
    
    for(int i = 0; i < n; i++) {
        is_completed[i] = 0;
    }
    
    while(completed != n) {
        int idx = -1;
        int min_burst = 9999;
        
        // Find process with minimum burst time that has arrived
        for(int i = 0; i < n; i++) {
            if(p[i].arrival_time <= current_time && !is_completed[i]) {
                if(p[i].burst_time < min_burst) {
                    min_burst = p[i].burst_time;
                    idx = i;
                }
                // If burst times are same, choose process with lower arrival time
                else if(p[i].burst_time == min_burst) {
                    if(p[i].arrival_time < p[idx].arrival_time) {
                        idx = i;
                    }
                }
            }
        }
        
        if(idx != -1) {
            p[idx].completion_time = current_time + p[idx].burst_time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
            
            current_time = p[idx].completion_time;
            is_completed[idx] = 1;
            completed++;
        }
        else {
            current_time++;
        }
    }
    
    printf("\n=== SJF (Non-Preemptive) Scheduling ===\n");
    displayProcesses(p, n);
    calculateAverage(p, n);
}

int main() {
    int n, choice;
    
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
        printf("2. SJF (Non-Preemptive) Scheduling\n");
        printf("3. Exit\n");
        printf("==========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                fcfs(processes_copy, n);
                break;
            case 2:
                sjf_non_preemptive(processes_copy, n);
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
