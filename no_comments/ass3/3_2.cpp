

#include <iostream>
using namespace std;

struct Process {
    int pid, at, bt, rt, ct, tat, wt;
};

void srtf(Process p[], int n) {
    cout << "\nSRTF Scheduling:\n";
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
    
    cout << "PID\tAT\tBT\tCT\tTAT\tWT\n";
    float avgTAT = 0, avgWT = 0;
    for(int i = 0; i < n; i++) {
        cout << p[i].pid << "\t" << p[i].at << "\t" << p[i].bt << "\t" 
             << p[i].ct << "\t" << p[i].tat << "\t" << p[i].wt << "\n";
        avgTAT += p[i].tat;
        avgWT += p[i].wt;
    }
    cout << "Avg TAT: " << avgTAT/n << " | Avg WT: " << avgWT/n << "\n";
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    
    Process p[n];
    for(int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "P" << p[i].pid << " - Arrival Time: ";
        cin >> p[i].at;
        cout << "P" << p[i].pid << " - Burst Time: ";
        cin >> p[i].bt;
    }
    
    srtf(p, n);
    return 0;
}