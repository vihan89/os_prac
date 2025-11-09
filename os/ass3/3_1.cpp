/*
==============================================================================
ASSIGNMENT 3.1: CPU SCHEDULING - FCFS & SJF (NON-PREEMPTIVE)
==============================================================================

PROBLEM:
- Implement FCFS (First Come First Serve) scheduling
- Implement SJF (Shortest Job First) Non-Preemptive scheduling
- Calculate: Completion Time, Turnaround Time, Waiting Time
- Display: Average TAT and Average WT
- Menu-driven program

COMPILATION:
g++ 3_1.cpp -o 3_1

EXECUTION:
./3_1

KEY FORMULAS TO REMEMBER:
1. Completion Time (CT) = Time at which process finishes execution
2. Turnaround Time (TAT) = CT - AT (Arrival Time)
3. Waiting Time (WT) = TAT - BT (Burst Time)
4. Average TAT = Sum of all TAT / Number of processes
5. Average WT = Sum of all WT / Number of processes

SAMPLE RUN:
Enter number of processes: 3
P1: AT=0, BT=4
P2: AT=1, BT=3  
P3: AT=2, BT=1

FCFS: Avg TAT=5.00, Avg WT=2.33
SJF:  Avg TAT=4.33, Avg WT=1.67
==============================================================================
*/

#include <iostream>
#include <iomanip>      // For formatted output

using namespace std;

#define MAX 100

// ============================================================================
// PROCESS STRUCTURE
// Stores all information about a process
// ============================================================================
struct Process {
    int pid;            // Process ID
    int arrivalTime;    // Arrival Time (AT)
    int burstTime;      // Burst Time (BT)
    int completionTime; // Completion Time (CT)
    int turnaroundTime; // Turnaround Time (TAT)
    int waitingTime;    // Waiting Time (WT)
};

// ============================================================================
// CALCULATE TIMES
// CT, TAT, WT calculation
// ============================================================================
void calculateTimes(Process p[], int n) {
    for(int i = 0; i < n; i++) {
        // Turnaround Time = CT - AT
        p[i].turnaroundTime = p[i].completionTime - p[i].arrivalTime;
        
        // Waiting Time = TAT - BT
        p[i].waitingTime = p[i].turnaroundTime - p[i].burstTime;
    }
}

// ============================================================================
// DISPLAY PROCESS TABLE
// Shows all process details in tabular format
// ============================================================================
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
    
    // Display averages
    cout << fixed << setprecision(2);
    cout << "\nAverage Turnaround Time: " << totalTAT/n << endl;
    cout << "Average Waiting Time: " << totalWT/n << endl;
}

// ============================================================================
// FCFS (FIRST COME FIRST SERVE) SCHEDULING
// Non-Preemptive: Processes execute in arrival order
// ============================================================================
void fcfs(Process p[], int n) {
    cout << "\n========================================" << endl;
    cout << "  FCFS SCHEDULING" << endl;
    cout << "========================================" << endl;
    
    // Sort by arrival time
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
        // If CPU is idle, jump to next arrival
        if(currentTime < p[i].arrivalTime) {
            currentTime = p[i].arrivalTime;
        }
        
        cout << "P" << p[i].pid << " ";
        
        // Completion time = current time + burst time
        p[i].completionTime = currentTime + p[i].burstTime;
        currentTime = p[i].completionTime;
    }
    cout << endl;
    
    calculateTimes(p, n);
    displayTable(p, n);
}

// ============================================================================
// SJF (SHORTEST JOB FIRST) NON-PREEMPTIVE SCHEDULING
// Select shortest burst time among arrived processes
// ============================================================================
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
        
        // Find process with shortest burst time among arrived processes
        for(int i = 0; i < n; i++) {
            if(!visited[i] && p[i].arrivalTime <= currentTime) {
                if(p[i].burstTime < minBurst) {
                    minBurst = p[i].burstTime;
                    shortestJob = i;
                }
                // If burst times are equal, choose earlier arrival
                else if(p[i].burstTime == minBurst) {
                    if(p[i].arrivalTime < p[shortestJob].arrivalTime) {
                        shortestJob = i;
                    }
                }
            }
        }
        
        if(shortestJob == -1) {
            // No process ready, CPU idle
            currentTime++;
        }
        else {
            // Execute the shortest job
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

// ============================================================================
// INPUT PROCESS DETAILS
// ============================================================================
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

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    Process processes[MAX];
    Process tempProcesses[MAX];  // For algorithm copy
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
        
        // Create copy of processes for algorithm
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
                // FCFS
                for(int i = 0; i < n; i++) tempProcesses[i] = processes[i];
                fcfs(tempProcesses, n);
                
                // SJF
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

/*
==============================================================================
IMPORTANT NOTES FOR EXAM:

1. FCFS (FIRST COME FIRST SERVE):
   - Simplest CPU scheduling algorithm
   - Processes executed in arrival order
   - Non-preemptive
   - Convoy Effect: Short processes wait for long processes
   - Average waiting time usually long
   
2. SJF (SHORTEST JOB FIRST):
   - Selects process with minimum burst time
   - Non-preemptive version: once started, runs to completion
   - Optimal: gives minimum average waiting time
   - Problem: Starvation - long processes may wait indefinitely
   - Difficult to know burst time in advance

3. KEY FORMULAS (MEMORIZE):
   CT = Completion Time (when process finishes)
   AT = Arrival Time (when process arrives)
   BT = Burst Time (CPU time needed)
   TAT = Turnaround Time = CT - AT
   WT = Waiting Time = TAT - BT = CT - AT - BT
   
   Average TAT = Σ(TAT) / n
   Average WT = Σ(WT) / n

4. GANTT CHART:
   - Visual representation of process execution
   - Shows which process runs at what time
   - Useful for manual calculation in exams

5. COMPARISON FCFS vs SJF:
   
   FCFS:
   + Simple to implement
   + No starvation
   - Long average waiting time
   - Convoy effect
   
   SJF:
   + Optimal (minimum average WT)
   + Better than FCFS
   - Starvation possible
   - Burst time prediction needed

6. EXAM TIPS:
   - Always draw Gantt chart first
   - Calculate CT for each process
   - Then calculate TAT = CT - AT
   - Then calculate WT = TAT - BT
   - Finally calculate averages
   - Show all steps for partial marks
==============================================================================
*/
