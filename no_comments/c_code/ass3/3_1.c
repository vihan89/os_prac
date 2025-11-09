#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid, at, bt, ct, tat, wt;
};

void fcfs(struct Process p[], int n) {
    printf("\n=== FCFS SCHEDULING ===\n");
    
    for(int i = 0; i < n-1; i++) {
        for(int j = i+1; j < n; j++) {
            if(p[i].at > p[j].at) {
                struct Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
    
    int time = 0;
    float totTAT = 0, totWT = 0;
    
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    
    for(int i = 0; i < n; i++) {
        if(time < p[i].at) time = p[i].at;
        
        p[i].ct = time + p[i].bt;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, 
               p[i].ct, p[i].tat, p[i].wt);
        
        totTAT += p[i].tat;
        totWT += p[i].wt;
        time = p[i].ct;
    }
    
    printf("\nAvg TAT: %.2f\nAvg WT: %.2f\n", totTAT/n, totWT/n);
}

void sjf(struct Process p[], int n) {
    printf("\n=== SJF SCHEDULING ===\n");
    
    int done[100] = {0};
    int completed = 0, time = 0;
    float totTAT = 0, totWT = 0;
    
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    
    while(completed < n) {
        int idx = -1, minBT = 9999;
        
        for(int i = 0; i < n; i++) {
            if(!done[i] && p[i].at <= time && p[i].bt < minBT) {
                minBT = p[i].bt;
                idx = i;
            }
        }
        
        if(idx == -1) {
            time++;
        } else {
            p[idx].ct = time + p[idx].bt;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            
            printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[idx].pid, p[idx].at, p[idx].bt, 
                   p[idx].ct, p[idx].tat, p[idx].wt);
            
            totTAT += p[idx].tat;
            totWT += p[idx].wt;
            time = p[idx].ct;
            done[idx] = 1;
            completed++;
        }
    }
    
    printf("\nAvg TAT: %.2f\nAvg WT: %.2f\n", totTAT/n, totWT/n);
}

int main() {
    struct Process p[100], temp[100];
    int n, choice;
    
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    for(int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("P%d - Arrival Time: ", p[i].pid);
        scanf("%d", &p[i].at);
        printf("P%d - Burst Time: ", p[i].pid);
        scanf("%d", &p[i].bt);
    }
    
    printf("\n1. FCFS\n2. SJF\nChoice: ");
    scanf("%d", &choice);
    
    for(int i = 0; i < n; i++) temp[i] = p[i];
    
    if(choice == 1)
        fcfs(temp, n);
    else if(choice == 2)
        sjf(temp, n);
    
    return 0;
}
