

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
};

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
        
        cout << "| P" << p[i].pid << "(" << p[i].burstTime << ") ";
        p[i].completionTime = currentTime + p[i].burstTime;
        currentTime = p[i].completionTime;
    }
    cout << "|" << endl;
    
    calculateTimes(p, n);
    displayTable(p, n);
}

void roundRobin(Process p[], int n) {
    cout << "\n========================================" << endl;
    cout << "  ROUND ROBIN SCHEDULING (TQ = 2)" << endl;
    cout << "========================================" << endl;
    
    const int TIME_QUANTUM = 2;
    

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
    

    while(completed < n) {
        if(readyQueue.isEmpty()) {

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
        

        int execTime = (p[current].remainingTime < TIME_QUANTUM) ? 
                       p[current].remainingTime : TIME_QUANTUM;
        
        cout << "P" << p[current].pid << "(" << execTime << ") | ";
        
        p[current].remainingTime -= execTime;
        currentTime += execTime;
        

        for(int i = 0; i < n; i++) {
            if(!inQueue[i] && p[i].arrivalTime <= currentTime && 
               p[i].remainingTime > 0) {
                readyQueue.enqueue(i);
                inQueue[i] = true;
            }
        }
        

        if(p[current].remainingTime == 0) {
            p[current].completionTime = currentTime;
            completed++;
        }
        else {

            readyQueue.enqueue(current);
        }
    }
    
    cout << endl;
    
    calculateTimes(p, n);
    displayTable(p, n);
    
    cout << "\nTime Quantum Used: " << TIME_QUANTUM << endl;
    cout << "Note: Processes execute in round-robin fashion" << endl;
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

