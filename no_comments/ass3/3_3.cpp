

#include <iostream>
#include <queue>
using namespace std;

struct Process {
    int pid, at, bt, rt, ct, tat, wt;
};

void roundRobin(Process p[], int n, int tq) {
    cout << "\nRound Robin (Time Quantum = " << tq << "):\n";
    for(int i = 0; i < n; i++) p[i].rt = p[i].bt;
    
    queue<int> q;
    bool inQ[100] = {false};
    int t = 0, done = 0;
    
    for(int i = 0; i < n; i++) {
        if(p[i].at <= t) {
            q.push(i);
            inQ[i] = true;
        }
    }
    
    while(done < n) {
        if(q.empty()) {
            t++;
            for(int i = 0; i < n; i++) {
                if(p[i].at <= t && !inQ[i] && p[i].rt > 0) {
                    q.push(i);
                    inQ[i] = true;
                }
            }
            continue;
        }
        
        int curr = q.front();
        q.pop();
        
        int exec = (p[curr].rt < tq) ? p[curr].rt : tq;
        p[curr].rt -= exec;
        t += exec;
        
        for(int i = 0; i < n; i++) {
            if(p[i].at <= t && !inQ[i] && p[i].rt > 0) {
                q.push(i);
                inQ[i] = true;
            }
        }
        
        if(p[curr].rt == 0) {
            p[curr].ct = t;
            p[curr].tat = p[curr].ct - p[curr].at;
            p[curr].wt = p[curr].tat - p[curr].bt;
            done++;
        } else {
            q.push(curr);
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
    int n, tq;
    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter time quantum: ";
    cin >> tq;
    
    Process p[n];
    for(int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "P" << p[i].pid << " - Arrival Time: ";
        cin >> p[i].at;
        cout << "P" << p[i].pid << " - Burst Time: ";
        cin >> p[i].bt;
    }
    
    roundRobin(p, n, tq);
    return 0;
}