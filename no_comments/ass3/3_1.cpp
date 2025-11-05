

#include <iostream>
using namespace std;

struct Process {
    int pid, at, bt, ct, tat, wt;
};

void fcfs(Process p[], int n) {
    cout << "\n=== FCFS SCHEDULING ===" << endl;
    
    // Sort by arrival time
    for(int i = 0; i < n-1; i++) {
        for(int j = i+1; j < n; j++) {
            if(p[i].at > p[j].at) {
                Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
    
    int time = 0;
    float totTAT = 0, totWT = 0;
    
    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    
    for(int i = 0; i < n; i++) {
        if(time < p[i].at) time = p[i].at;
        
        p[i].ct = time + p[i].bt;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        
        cout << "P" << p[i].pid << "\t" << p[i].at << "\t" << p[i].bt << "\t" 
             << p[i].ct << "\t" << p[i].tat << "\t" << p[i].wt << endl;
        
        totTAT += p[i].tat;
        totWT += p[i].wt;
        time = p[i].ct;
    }
    
    cout << "\nAvg TAT: " << totTAT/n << "\nAvg WT: " << totWT/n << endl;
}

void sjf(Process p[], int n) {
    cout << "\n=== SJF SCHEDULING ===" << endl;
    
    int done[100] = {0};
    int completed = 0, time = 0;
    float totTAT = 0, totWT = 0;
    
    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    
    while(completed < n) {
        int idx = -1, minBT = 9999;
        
        // Find shortest job that has arrived
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
            
            cout << "P" << p[idx].pid << "\t" << p[idx].at << "\t" << p[idx].bt << "\t" 
                 << p[idx].ct << "\t" << p[idx].tat << "\t" << p[idx].wt << endl;
            
            totTAT += p[idx].tat;
            totWT += p[idx].wt;
            time = p[idx].ct;
            done[idx] = 1;
            completed++;
        }
    }
    
    cout << "\nAvg TAT: " << totTAT/n << "\nAvg WT: " << totWT/n << endl;
}

int main() {
    Process p[100], temp[100];
    int n, choice;
    
    cout << "Enter number of processes: ";
    cin >> n;
    
    for(int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "P" << p[i].pid << " - Arrival Time: ";
        cin >> p[i].at;
        cout << "P" << p[i].pid << " - Burst Time: ";
        cin >> p[i].bt;
    }
    
    cout << "\n1. FCFS\n2. SJF\nChoice: ";
    cin >> choice;
    
    // Make copy
    for(int i = 0; i < n; i++) temp[i] = p[i];
    
    if(choice == 1)
        fcfs(temp, n);
    else if(choice == 2)
        sjf(temp, n);
    
    return 0;
}

