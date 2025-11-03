

#include <iostream>
#include <iomanip>

using namespace std;

#define MAX 100

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
};

void calculateTimes(Process p[], int n) {
    for(int i = 0; i < n; i++) {

        p[i].turnaroundTime = p[i].completionTime - p[i].arrivalTime;
        

        p[i].waitingTime = p[i].turnaroundTime - p[i].burstTime;
    }
}

void displayTable(Process p[], int n) {
    cout << "\n+-----+----+----+----+-----+-----+----+" << endl;
    cout << "| PID | AT | BT | CT | TAT |  WT | " << endl;
    cout << "+-----+----+----+----+-----+-----+----+" << endl;
    
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
    
    cout << "+-----+----+----+----+-----+-----+----+" << endl;
    

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
    
    cout << "\nExecution Order: ";
    for(int i = 0; i < n; i++) {

        if(currentTime < p[i].arrivalTime) {
            currentTime = p[i].arrivalTime;
        }
        
        cout << "P" << p[i].pid << " ";
        

        p[i].completionTime = currentTime + p[i].burstTime;
        currentTime = p[i].completionTime;
    }
    cout << endl;
    
    calculateTimes(p, n);
    displayTable(p, n);
}

void sjfNonPreemptive(Process p[], int n) {
    cout << "\n========================================" << endl;
    cout << "  SJF (NON-PREEMPTIVE) SCHEDULING" << endl;
    cout << "========================================" << endl;
    
    int completed = 0;
    int currentTime = 0;
    bool visited[MAX] = {false};
    
    cout << "\nExecution Order: ";
    
    while(completed < n) {
        int shortestJob = -1;
        int minBurst = 999999;
        

        for(int i = 0; i < n; i++) {
            if(!visited[i] && p[i].arrivalTime <= currentTime) {
                if(p[i].burstTime < minBurst) {
                    minBurst = p[i].burstTime;
                    shortestJob = i;
                }

                else if(p[i].burstTime == minBurst) {
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

            cout << "P" << p[shortestJob].pid << " ";
            
            p[shortestJob].completionTime = currentTime + p[shortestJob].burstTime;
            currentTime = p[shortestJob].completionTime;
            visited[shortestJob] = true;
            completed++;
        }
    }
    cout << endl;
    
    calculateTimes(p, n);
    displayTable(p, n);
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
    cout << "  FCFS & SJF (Non-Preemptive)" << endl;
    cout << "========================================" << endl;
    
    inputProcesses(processes, n);
    
    while(true) {
        cout << "\n========================================" << endl;
        cout << "           MENU" << endl;
        cout << "========================================" << endl;
        cout << "1. FCFS (First Come First Serve)" << endl;
        cout << "2. SJF (Shortest Job First - Non-Preemptive)" << endl;
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
                sjfNonPreemptive(tempProcesses, n);
                break;
                
            case 3: {

                for(int i = 0; i < n; i++) tempProcesses[i] = processes[i];
                fcfs(tempProcesses, n);
                

                for(int i = 0; i < n; i++) tempProcesses[i] = processes[i];
                sjfNonPreemptive(tempProcesses, n);
                break;
            }
                
            case 4:
                inputProcesses(processes, n);
                break;
                
            case 5:
                cout << "\nExiting program..." << endl;
                return 0;
                
            default:
                cout << "\nInvalid choice!" << endl;
        }
    }
    
    return 0;
}

