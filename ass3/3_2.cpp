/*
==============================================================================
ASSIGNMENT 3.2: CPU SCHEDULING - FCFS & SJF PREEMPTIVE (SRTF)
==============================================================================

PROBLEM:
- Implement FCFS (same as 3.1)
- Implement SJF Preemptive (SRTF - Shortest Remaining Time First)
- At each time unit, check if new process with shorter remaining time arrived
- Context switch if shorter process found
- Calculate CT, TAT, WT and display averages

COMPILATION:
g++ 3_2.cpp -o 3_2

EXECUTION:
./3_2

KEY CONCEPTS:
1. Preemptive: CPU can be taken away from running process
2. SRTF: At each moment, process with shortest remaining time executes
3. Context Switch: Switching from one process to another
4. Remaining Time: How much burst time is left for a process

FORMULAS:
- Completion Time = Time when process finishes
- TAT = CT - AT
- WT = TAT - BT
- Remaining Time = Original BT - Time already executed

SAMPLE RUN:
P1: AT=0, BT=8
P2: AT=1, BT=4
P3: AT=2, BT=2

SRTF will switch: P1 -> P2 -> P3 -> P2 -> P1
==============================================================================
*/

#include <iostream>
#include <iomanip>

using namespace std;

#define MAX 100

// ============================================================================
// PROCESS STRUCTURE
// ============================================================================
struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int remainingTime;      // For preemptive scheduling
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    int startTime;          // First time process gets CPU
    bool isStarted;         // Track if process has started
};

// ============================================================================
// CALCULATE TIMES
// ============================================================================
void calculateTimes(Process p[], int n) {
    for(int i = 0; i < n; i++) {
        p[i].turnaroundTime = p[i].completionTime - p[i].arrivalTime;
        p[i].waitingTime = p[i].turnaroundTime - p[i].burstTime;
    }
}

// ============================================================================
// DISPLAY TABLE
// ============================================================================
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

// ============================================================================
// FCFS SCHEDULING
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

// ============================================================================
// SJF PREEMPTIVE (SRTF) SCHEDULING
// Shortest Remaining Time First
// ============================================================================
void srtf(Process p[], int n) {
    cout << "\n========================================" << endl;
    cout << "  SRTF (SJF PREEMPTIVE) SCHEDULING" << endl;
    cout << "========================================" << endl;
    
    // Initialize remaining time
    for(int i = 0; i < n; i++) {
        p[i].remainingTime = p[i].burstTime;
        p[i].isStarted = false;
    }
    
    int currentTime = 0;
    int completed = 0;
    int lastProcess = -1;
    
    // Find maximum completion time
    int maxTime = 0;
    for(int i = 0; i < n; i++) {
        maxTime += p[i].burstTime;
    }
    maxTime += 100;  // Safety margin
    
    cout << "\nGantt Chart: ";
    
    // Run until all processes complete
    while(completed < n && currentTime < maxTime) {
        int shortestJob = -1;
        int minRemaining = 999999;
        
        // Find process with shortest remaining time among arrived processes
        for(int i = 0; i < n; i++) {
            if(p[i].arrivalTime <= currentTime && p[i].remainingTime > 0) {
                if(p[i].remainingTime < minRemaining) {
                    minRemaining = p[i].remainingTime;
                    shortestJob = i;
                }
                // Tie-breaker: earlier arrival time
                else if(p[i].remainingTime == minRemaining) {
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
            // Print Gantt chart (only when process changes)
            if(shortestJob != lastProcess) {
                if(lastProcess != -1) {
                    cout << "| ";
                }
                cout << "P" << p[shortestJob].pid << " ";
                lastProcess = shortestJob;
            }
            
            // Execute for 1 time unit
            p[shortestJob].remainingTime--;
            currentTime++;
            
            // Check if process completed
            if(p[shortestJob].remainingTime == 0) {
                p[shortestJob].completionTime = currentTime;
                completed++;
            }
        }
    }
    cout << "|" << endl;
    
    calculateTimes(p, n);
    displayTable(p, n);
    
    // Show context switches
    cout << "\nNote: SRTF may cause multiple context switches" << endl;
    cout << "This provides better average waiting time but overhead increases" << endl;
}

// ============================================================================
// INPUT PROCESSES
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
        
        // Create copy
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

/*
==============================================================================
IMPORTANT NOTES FOR EXAM:

1. SRTF (SHORTEST REMAINING TIME FIRST):
   - Preemptive version of SJF
   - At each time unit, check all arrived processes
   - Select process with minimum remaining time
   - Can preempt currently running process
   - Optimal: Gives minimum average waiting time

2. PREEMPTIVE vs NON-PREEMPTIVE:
   
   Non-Preemptive (SJF):
   - Once process starts, runs to completion
   - No interruption
   - Less context switches
   
   Preemptive (SRTF):
   - Process can be interrupted
   - CPU given to shorter process
   - More context switches
   - Better average WT

3. ALGORITHM STEPS (SRTF):
   a) Initialize remaining_time = burst_time for all
   b) For each time unit:
      - Find arrived process with min remaining_time
      - Execute it for 1 unit
      - Decrease remaining_time by 1
      - If remaining_time = 0, mark as completed
   c) Continue until all completed

4. CONTEXT SWITCH:
   - Overhead of switching between processes
   - Involves saving/restoring process state
   - Too many switches reduce efficiency
   - SRTF may have many context switches

5. STARVATION:
   - Long processes may never execute
   - Short processes keep arriving
   - Solution: Aging (increase priority over time)

6. EXAM CALCULATION TIPS:
   - Create table: Time | Running Process | Remaining Times
   - At each time, check all arrived processes
   - Select minimum remaining time
   - Update table row by row
   - Mark completion times
   - Calculate TAT and WT

7. EXAMPLE CALCULATION:
   P1: AT=0, BT=8
   P2: AT=1, BT=4
   P3: AT=2, BT=2
   
   Time | Process | P1_rem | P2_rem | P3_rem
   0    | P1      | 8      | -      | -
   1    | P1      | 7      | 4      | -
   2    | P1      | 6      | 4      | 2
   2    | P3      | 6      | 4      | 2  (switch!)
   3    | P3      | 6      | 4      | 1
   4    | P3 done | 6      | 4      | 0
   4    | P2      | 6      | 4      | 0
   ...continue
   
8. KEY DIFFERENCE FROM 3.1:
   - 3.1: SJF Non-Preemptive (no interruption)
   - 3.2: SRTF Preemptive (can interrupt)
==============================================================================
*/
