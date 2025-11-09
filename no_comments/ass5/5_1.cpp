// Banker's Algorithm - Safety Check (Simple version)
#include <iostream>
using namespace std;

int main() {
    int n, m;
    int avail[10], max_m[10][10], alloc[10][10], need[10][10];
    
    // Input
    cout << "Processes: "; cin >> n;
    cout << "Resources: "; cin >> m;
    
    cout << "Available: ";
    for(int i = 0; i < m; i++) cin >> avail[i];
    
    cout << "Max matrix:\n";
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++) cin >> max_m[i][j];
    
    cout << "Allocation matrix:\n";
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++) cin >> alloc[i][j];
    
    // Calculate Need = Max - Allocation
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            need[i][j] = max_m[i][j] - alloc[i][j];
    
    // Safety Algorithm
    int work[10], finish[10] = {0}, safe[10], count = 0;
    for(int i = 0; i < m; i++) work[i] = avail[i];
    
    // Find safe sequence
    while(count < n) {
        bool found = false;
        
        for(int i = 0; i < n; i++) {
            if(finish[i] == 0) {
                // Check if Need[i] <= Work
                bool canRun = true;
                for(int j = 0; j < m; j++) {
                    if(need[i][j] > work[j]) {
                        canRun = false;
                        break;
                    }
                }
                
                if(canRun) {
                    // Allocate resources: Work = Work + Alloc[i]
                    for(int j = 0; j < m; j++)
                        work[j] += alloc[i][j];
                    
                    safe[count++] = i;
                    finish[i] = 1;
                    found = true;
                    cout << "P" << i << " executes (Work: ";
                    for(int j = 0; j < m; j++) cout << work[j] << " ";
                    cout << ")\n";
                }
            }
        }
        
        if(!found) {
            cout << "\nState is UNSAFE!\n";
            cout << "Reason: No process can execute with available resources.\n";
            cout << "Remaining processes: ";
            for(int i = 0; i < n; i++)
                if(!finish[i]) cout << "P" << i << " ";
            cout << "\nCurrent Work: ";
            for(int i = 0; i < m; i++) cout << work[i] << " ";
            cout << "\n** Deadlock detected! **\n";
            return 0;
        }
    }
    
    // Print safe sequence
    cout << "\nState is SAFE!\n";
    cout << "Safe sequence: ";
    for(int i = 0; i < n; i++)
        cout << "P" << safe[i] << " ";
    cout << "\nAll processes can complete successfully.\n";
    
    return 0;
}