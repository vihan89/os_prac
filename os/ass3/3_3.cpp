/*
==============================================================================
ASSIGNMENT 3.3: CPU SCHEDULING - FCFS & ROUND ROBIN
==============================================================================

PROBLEM:
- Implement FCFS (same as previous)
- Implement Round Robin with Time Quantum = 2
- Use queue to manage process execution
- Calculate CT, TAT, WT and display averages

COMPILATION:
g++ 3_3.cpp -o 3_3

EXECUTION:
./3_3

KEY CONCEPTS:
1. Round Robin: Cyclic scheduling with fixed time quantum
2. Time Quantum (TQ): Maximum time a process can run before preemption
3. Ready Queue: FIFO queue of ready processes
4. Preemptive: Process runs for TQ or until completion
5. Fair scheduling: All processes get equal CPU time

FORMULAS:
- If remaining_time > TQ: Execute for TQ, add back to queue
- If remaining_time <= TQ: Execute fully, process completes
- TAT = CT - AT
- WT = TAT - BT

SAMPLE RUN:
P1: AT=0, BT=5, P2: AT=1, BT=3, P3: AT=2, BT=1
TQ=2: P1(2) -> P2(2) -> P3(1) -> P1(2) -> P2(1) -> P1(1)
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
    int remainingTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
};

// ============================================================================
// SIMPLE QUEUE IMPLEMENTATION
// For Round Robin ready queue
// ============================================================================
struct Queue {
    int items[MAX];
    int front, rear;
    
    void initialize() {
        front = -1;
        rear = -1;
    }
    
    bool isEmpty() {
        return (front == -1);
    }
    
    void enqueue(int value) {
        if(front == -1) {
            front = 0;
        }
        rear++;
        items[rear] = value;
    }
    
    int dequeue() {
        int item = items[front];
        if(front == rear) {
            front = -1;
            rear = -1;
        }
        else {
            front++;
        }
        return item;
    }
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
        
        cout << "| P" << p[i].pid << "(" << p[i].burstTime << ") ";
        p[i].completionTime = currentTime + p[i].burstTime;
        currentTime = p[i].completionTime;
    }
    cout << "|" << endl;
    
    calculateTimes(p, n);
    displayTable(p, n);
}

// ============================================================================
// ROUND ROBIN SCHEDULING
// Time Quantum = 2
// ============================================================================
void roundRobin(Process p[], int n) {
    cout << "\n========================================" << endl;
    cout << "  ROUND ROBIN SCHEDULING (TQ = 2)" << endl;
    cout << "========================================" << endl;
    
    const int TIME_QUANTUM = 2;
    
    // Initialize remaining time
    for(int i = 0; i < n; i++) {
        p[i].remainingTime = p[i].burstTime;
    }
    
    Queue readyQueue;
    readyQueue.initialize();
    
    int currentTime = 0;
    int completed = 0;
    bool inQueue[MAX] = {false};
    
    cout << "\nGantt Chart: ";
    cout << "| ";
    
    // Add first arrived process to queue
    int minArrival = 999999;
    int firstProcess = -1;
    for(int i = 0; i < n; i++) {
        if(p[i].arrivalTime < minArrival) {
            minArrival = p[i].arrivalTime;
            firstProcess = i;
        }
    }
    
    if(firstProcess != -1) {
        currentTime = p[firstProcess].arrivalTime;
        readyQueue.enqueue(firstProcess);
        inQueue[firstProcess] = true;
    }
    
    // Process queue
    while(completed < n) {
        if(readyQueue.isEmpty()) {
            // No process ready, find next arrival
            int nextArrival = 999999;
            int nextProcess = -1;
            for(int i = 0; i < n; i++) {
                if(!inQueue[i] && p[i].remainingTime > 0 && 
                   p[i].arrivalTime < nextArrival && p[i].arrivalTime > currentTime) {
                    nextArrival = p[i].arrivalTime;
                    nextProcess = i;
                }
            }
            if(nextProcess != -1) {
                currentTime = p[nextProcess].arrivalTime;
                readyQueue.enqueue(nextProcess);
                inQueue[nextProcess] = true;
            }
            continue;
        }
        
        int current = readyQueue.dequeue();
        
        // Execute for time quantum or remaining time (whichever is smaller)
        int execTime = (p[current].remainingTime < TIME_QUANTUM) ? 
                       p[current].remainingTime : TIME_QUANTUM;
        
        cout << "P" << p[current].pid << "(" << execTime << ") | ";
        
        p[current].remainingTime -= execTime;
        currentTime += execTime;
        
        // Add newly arrived processes to queue
        for(int i = 0; i < n; i++) {
            if(!inQueue[i] && p[i].arrivalTime <= currentTime && 
               p[i].remainingTime > 0) {
                readyQueue.enqueue(i);
                inQueue[i] = true;
            }
        }
        
        // If process completed
        if(p[current].remainingTime == 0) {
            p[current].completionTime = currentTime;
            completed++;
        }
        else {
            // Add back to queue if not completed
            readyQueue.enqueue(current);
        }
    }
    
    cout << endl;
    
    calculateTimes(p, n);
    displayTable(p, n);
    
    cout << "\nTime Quantum Used: " << TIME_QUANTUM << endl;
    cout << "Note: Processes execute in round-robin fashion" << endl;
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
    cout << "  FCFS & Round Robin (TQ=2)" << endl;
    cout << "========================================" << endl;
    
    inputProcesses(processes, n);
    
    while(true) {
        cout << "\n========================================" << endl;
        cout << "           MENU" << endl;
        cout << "========================================" << endl;
        cout << "1. FCFS (First Come First Serve)" << endl;
        cout << "2. Round Robin (Time Quantum = 2)" << endl;
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
                roundRobin(tempProcesses, n);
                break;
                
            case 3:
                for(int i = 0; i < n; i++) tempProcesses[i] = processes[i];
                fcfs(tempProcesses, n);
                
                for(int i = 0; i < n; i++) tempProcesses[i] = processes[i];
                roundRobin(tempProcesses, n);
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

1. ROUND ROBIN SCHEDULING:
   - Preemptive scheduling
   - Designed for time-sharing systems
   - Each process gets small unit of CPU time (time quantum)
   - After quantum expires, process preempted and added to ready queue
   - Fair allocation of CPU time

2. TIME QUANTUM (TQ):
   - Fixed time slice for each process
   - Too large TQ → behaves like FCFS
   - Too small TQ → too many context switches
   - Typical: 10-100 milliseconds
   - In this program: TQ = 2

3. ALGORITHM STEPS:
   a) Maintain ready queue (FIFO)
   b) Add first arrived process to queue
   c) While queue not empty:
      - Dequeue process
      - Execute for min(TQ, remaining_time)
      - Add newly arrived processes to queue
      - If not complete, enqueue back
      - Else, mark completion
   d) Calculate TAT and WT

4. READY QUEUE:
   - FIFO (First In First Out) queue
   - Processes wait in queue for CPU
   - After executing for TQ, go to end of queue
   - New arrivals added to queue

5. ADVANTAGES:
   + Fair to all processes
   + No starvation
   + Good response time for interactive systems
   + Easy to implement

6. DISADVANTAGES:
   - Performance depends on TQ selection
   - More context switches than FCFS
   - Average waiting time often high
   - Overhead of maintaining queue

7. PERFORMANCE METRICS:
   - Response Time: Time from arrival to first execution
   - Waiting Time: Total time in ready queue
   - Turnaround Time: Total time in system
   - RR gives better response time than FCFS

8. EXAM CALCULATION:
   P1: AT=0, BT=5
   P2: AT=1, BT=3
   P3: AT=2, BT=1
   TQ = 2
   
   Time | Queue | Action
   0    | [P1]  | P1 executes (0-2)
   2    | [P2,P3,P1] | P2 arrives at 1, P3 at 2
   2    | [P2,P3,P1] | P2 executes (2-4)
   4    | [P3,P1,P2] | P3 executes (4-5), completes
   5    | [P1,P2] | P1 executes (5-7)
   7    | [P2,P1] | P2 executes (7-8), completes
   8    | [P1]  | P1 executes (8-9), completes
   
   CT: P1=9, P2=8, P3=5

9. COMPARISON: FCFS vs ROUND ROBIN
   
   FCFS:
   + Simple, no overhead
   + Good for batch systems
   - Convoy effect
   - Poor response time
   
   Round Robin:
   + Fair sharing
   + Good response time
   + No starvation
   - Context switch overhead
   - Need to choose TQ wisely

10. CHOOSING TIME QUANTUM:
    - 80% of CPU bursts should be < TQ
    - If TQ → ∞, becomes FCFS
    - If TQ → 0, too many switches
    - Rule of thumb: TQ = 10-100ms
==============================================================================
*/
