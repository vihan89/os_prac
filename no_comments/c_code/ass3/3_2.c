#include <stdio.h>

struct Process {
    int pid, at, bt, rt, ct, tat, wt;
};

void srtf(struct Process p[], int n) {
    printf("\nSRTF Scheduling:\n");
    for(int i = 0; i < n; i++) p[i].rt = p[i].bt;
    
    int t = 0, done = 0;
    while(done < n) {
        int shortest = -1, minRT = 9999;
        for(int i = 0; i < n; i++) {
            if(p[i].at <= t && p[i].rt > 0 && p[i].rt < minRT) {
                minRT = p[i].rt;
                shortest = i;
            }
        }
        if(shortest == -1) { t++; continue; }
        
        p[shortest].rt--;
        t++;
        if(p[shortest].rt == 0) {
            p[shortest].ct = t;
            p[shortest].tat = p[shortest].ct - p[shortest].at;
            p[shortest].wt = p[shortest].tat - p[shortest].bt;
            done++;
        }
    }
    
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    float avgTAT = 0, avgWT = 0;
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, 
               p[i].ct, p[i].tat, p[i].wt);
        avgTAT += p[i].tat;
        avgWT += p[i].wt;
    }
    printf("Avg TAT: %.2f | Avg WT: %.2f\n", avgTAT/n, avgWT/n);
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    struct Process p[n];
    for(int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("P%d - Arrival Time: ", p[i].pid);
        scanf("%d", &p[i].at);
        printf("P%d - Burst Time: ", p[i].pid);
        scanf("%d", &p[i].bt);
    }
    
    srtf(p, n);
    return 0;
}
