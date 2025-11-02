// Assignment 1 - Problem 3: Priority CPU Scheduling
#include <stdio.h>

int main() {
    int n, i, j, temp;
    float avg_wt = 0, avg_tat = 0;
    
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    int bt[n], wt[n], tat[n], p[n], priority[n];
    
    printf("Enter burst time and priority for each process:\n");
    for(i = 0; i < n; i++) {
        printf("P%d Burst Time: ", i+1);
        scanf("%d", &bt[i]);
        printf("P%d Priority: ", i+1);
        scanf("%d", &priority[i]);
        p[i] = i + 1;
    }
    
    // Sort by priority (lower number = higher priority)
    for(i = 0; i < n-1; i++) {
        for(j = i+1; j < n; j++) {
            if(priority[i] > priority[j]) {
                temp = priority[i];
                priority[i] = priority[j];
                priority[j] = temp;
                
                temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;
                
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
    
    // Calculate waiting time
    wt[0] = 0;
    for(i = 1; i < n; i++) {
        wt[i] = wt[i-1] + bt[i-1];
    }
    
    // Calculate turnaround time
    for(i = 0; i < n; i++) {
        tat[i] = wt[i] + bt[i];
    }
    
    // Display results
    printf("\nProcess\tPriority\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for(i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\n", p[i], priority[i], bt[i], wt[i], tat[i]);
        avg_wt += wt[i];
        avg_tat += tat[i];
    }
    
    printf("\nAverage Waiting Time: %.2f", avg_wt/n);
    printf("\nAverage Turnaround Time: %.2f\n", avg_tat/n);
    
    return 0;
}
