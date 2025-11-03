

#include <iostream>
#include <iomanip>

using namespace std;

#define MAX 100

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    int startTime;
    bool isStarted;
};

void calculateTimes(Process p[], int n) {
    for(int i = 0; i < n; i++) {
        p[i].turnaroundTime = p[i].completionTime - p[i].arrivalTime;
        p[i].waitingTime = p[i].turnaroundTime - p[i].burstTime;
    }
}

void displayTable(Process p[], int n) {
    cout << "\n+-----+----+----+----+-----+-----+" << endl;
    cout << "| PID | AT | BT | CT | TAT |  WT |" << endl;
    cout << "+-----+----+----+----+-----+-----+" << endl;
    
    float totalTAT = 0, totalWT = 0;
    
    for(int i = 0; i < n; i++) {
        cout << "|  P" << setw(2) << p[i].pid << " | "
             << setw(2) << p[i].arrivalTime << " | "
             << setw(2) << p[i].burstTime << " | "
             << setw(2) << p[i].completionTime << " | "
             << setw(3) << p[i].turnaroundTime << " | "
             << setw(3) << p[i].waitingTime << " |" << endl;
        
        totalTAT += p[i].turnaroundTime;
        totalWT += p[i].waitingTime;
    }
    
    cout << "+-----+----+----+----+-----+-----+" << endl;
    
    cout << fixed << setprecision(2);
    cout << "\nAverage Turnaround Time: " << totalTAT/n << endl;
    cout << "Average Waiting Time: " << totalWT/n << endl;
}

void fcfs(Process p[], int n) {
    cout << "\n========================================" << endl;
    cout << "  FCFS SCHEDULING" << endl;
    cout << "========================================" << endl;
    

    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(p[j].arrivalTime > p[j+1].arrivalTime) {
                Process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }
    
    int currentTime = 0;
    
    cout << "\nGantt Chart: ";
    for(int i = 0; i < n; i++) {
        if(currentTime < p[i].arrivalTime) {
            currentTime = p[i].arrivalTime;
        }
        
        cout << "| P" << p[i].pid << " ";
        p[i].completionTime = currentTime + p[i].burstTime;
        currentTime = p[i].completionTime;
    }
    cout << "|" << endl;
    
    calculateTimes(p, n);
    displayTable(p, n);
}

void srtf(Process p[], int n) {
    cout << "\n========================================" << endl;
    cout << "  SRTF (SJF PREEMPTIVE) SCHEDULING" << endl;
    cout << "========================================" << endl;
    

    for(int i = 0; i < n; i++) {
        p[i].remainingTime = p[i].burstTime;
        p[i].isStarted = false;
    }
    
    int currentTime = 0;
    int completed = 0;
    int lastProcess = -1;
    

    int maxTime = 0;
    for(int i = 0; i < n; i++) {
        maxTime += p[i].burstTime;
    }
    maxTime += 100;
    
    cout << "\nGantt Chart: ";
    

    while(completed < n && currentTime < maxTime) {
        int shortestJob = -1;
        int minRemaining = 999999;
        

        for(int i = 0; i < n; i++) {
            if(p[i].arrivalTime <= currentTime && p[i].remainingTime > 0) {
                if(p[i].remainingTime < minRemaining) {
                    minRemaining = p[i].remainingTime;
                    shortestJob = i;
                }

                else if(p[i].remainingTime == minRemaining) {
                    if(p[i].arrivalTime < p[shortestJob].arrivalTime) {
                        shortestJob = i;
                    }
                }
            }
        }
        
        if(shortestJob == -1) {

            currentTime++;
        }
        else {

            if(shortestJob != lastProcess) {
                if(lastProcess != -1) {
                    cout << "| ";
                }
                cout << "P" << p[shortestJob].pid << " ";
                lastProcess = shortestJob;
            }
            

            p[shortestJob].remainingTime--;
            currentTime++;
            

            if(p[shortestJob].remainingTime == 0) {
                p[shortestJob].completionTime = currentTime;
                completed++;
            }
        }
    }
    cout << "|" << endl;
    
    calculateTimes(p, n);
    displayTable(p, n);
    

    cout << "\nNote: SRTF may cause multiple context switches" << endl;
    cout << "This provides better average waiting time but overhead increases" << endl;
}

void inputProcesses(Process p[], int &n) {
    cout << "\nEnter number of processes (max " << MAX << "): ";
    cin >> n;
    
    if(n <= 0 || n > MAX) {
        cout << "Invalid input!" << endl;
        exit(1);
    }
    
    for(int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "\nProcess P" << p[i].pid << ":" << endl;
        cout << "  Arrival Time: ";
        cin >> p[i].arrivalTime;
        cout << "  Burst Time: ";
        cin >> p[i].burstTime;
    }
}

int main() {
    Process processes[MAX];
    Process tempProcesses[MAX];
    int n;
    int choice;
    
    cout << "========================================" << endl;
    cout << "  CPU SCHEDULING ALGORITHMS" << endl;
    cout << "  FCFS & SRTF (SJF Preemptive)" << endl;
    cout << "========================================" << endl;
    
    inputProcesses(processes, n);
    
    while(true) {
        cout << "\n========================================" << endl;
        cout << "           MENU" << endl;
        cout << "========================================" << endl;
        cout << "1. FCFS (First Come First Serve)" << endl;
        cout << "2. SRTF (Shortest Remaining Time First)" << endl;
        cout << "3. Compare Both Algorithms" << endl;
        cout << "4. Re-enter Process Data" << endl;
        cout << "5. Exit" << endl;
        cout << "========================================" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        

        for(int i = 0; i < n; i++) {
            tempProcesses[i] = processes[i];
        }
        
        switch(choice) {
            case 1:
                fcfs(tempProcesses, n);
                break;
                
            case 2:
                srtf(tempProcesses, n);
                break;
                
            case 3:
                for(int i = 0; i < n; i++) tempProcesses[i] = processes[i];
                fcfs(tempProcesses, n);
                
                for(int i = 0; i < n; i++) tempProcesses[i] = processes[i];
                srtf(tempProcesses, n);
                break;
                
            case 4:
                inputProcesses(processes, n);
                break;
                
            case 5:
                cout << "\nExiting..." << endl;
                return 0;
                
            default:
                cout << "\nInvalid choice!" << endl;
        }
    }
    
    return 0;
}

