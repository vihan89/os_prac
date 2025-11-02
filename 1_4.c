// Assignment 1 - Problem 4: Round Robin CPU Scheduling
#include <stdio.h>

int main() {
    int n, i, time = 0, remain, quantum;
    float avg_wt = 0, avg_tat = 0;
    
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    int bt[n], rem_bt[n], wt[n], tat[n];
    
    printf("Enter burst time for each process:\n");
    for(i = 0; i < n; i++) {
        printf("P%d: ", i+1);
        scanf("%d", &bt[i]);
        rem_bt[i] = bt[i];
        wt[i] = 0;
    }
    
    printf("Enter time quantum: ");
    scanf("%d", &quantum);
    
    remain = n;
    
    // Process scheduling
    while(remain != 0) {
        for(i = 0; i < n; i++) {
            if(rem_bt[i] > 0) {
                if(rem_bt[i] <= quantum) {
                    time += rem_bt[i];
                    rem_bt[i] = 0;
                    remain--;
                    tat[i] = time;
                } else {
                    time += quantum;
                    rem_bt[i] -= quantum;
                }
            }
        }
    }
    
    // Calculate waiting time
    for(i = 0; i < n; i++) {
        wt[i] = tat[i] - bt[i];
    }
    
    // Display results
    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for(i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\n", i+1, bt[i], wt[i], tat[i]);
        avg_wt += wt[i];
        avg_tat += tat[i];
    }
    
    printf("\nAverage Waiting Time: %.2f", avg_wt/n);
    printf("\nAverage Turnaround Time: %.2f\n", avg_tat/n);
    
    return 0;
}
