/*
3.2 Write a menu-driven C program to simulate the following CPU scheduling algorithms 
to find average turnaround time and average waiting time.
a) FCFS
b) SJF (preemptive)
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

// SJF Preemptive (SRTF) Scheduling Algorithm
void sjf_preemptive(Process p[], int n) {
    int completed = 0, current_time = 0;
    int min_remaining_time, shortest;
    int finish_time;
    int check = 0;
    
    // Initialize remaining time
    for(int i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
    }
    
    while(completed != n) {
        shortest = -1;
        min_remaining_time = 9999;
        
        // Find process with minimum remaining time among arrived processes
        for(int i = 0; i < n; i++) {
            if(p[i].arrival_time <= current_time && p[i].remaining_time > 0) {
                if(p[i].remaining_time < min_remaining_time) {
                    min_remaining_time = p[i].remaining_time;
                    shortest = i;
                }
                // If remaining times are same, choose process with lower arrival time
                else if(p[i].remaining_time == min_remaining_time) {
                    if(p[i].arrival_time < p[shortest].arrival_time) {
                        shortest = i;
                    }
                }
            }
        }
        
        if(shortest == -1) {
            current_time++;
            continue;
        }
        
        // Process the selected process for 1 unit of time
        p[shortest].remaining_time--;
        current_time++;
        
        // If process is completed
        if(p[shortest].remaining_time == 0) {
            completed++;
            finish_time = current_time;
            
            p[shortest].completion_time = finish_time;
            p[shortest].turnaround_time = p[shortest].completion_time - p[shortest].arrival_time;
            p[shortest].waiting_time = p[shortest].turnaround_time - p[shortest].burst_time;
        }
    }
    
    printf("\n=== SJF (Preemptive / SRTF) Scheduling ===\n");
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
        printf("2. SJF (Preemptive / SRTF) Scheduling\n");
        printf("3. Exit\n");
        printf("==========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                fcfs(processes_copy, n);
                break;
            case 2:
                sjf_preemptive(processes_copy, n);
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
