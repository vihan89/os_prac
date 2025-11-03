/*
==============================================================================
ASSIGNMENT 5.1: BANKER'S ALGORITHM - BASIC SAFETY CHECK
==============================================================================

PROBLEM:
- Input: Available resources, Max matrix, Allocation matrix
- Calculate Need matrix
- Implement safety algorithm
- Find safe sequence if exists
- Display why unsafe if not safe

COMPILATION:
g++ 5_1.cpp -o 5_1

EXECUTION:
./5_1

KEY FORMULAS:
1. Need[i][j] = Max[i][j] - Allocation[i][j]
2. Safe State: There exists a sequence of processes that can complete
3. Process can run if: Need[i] <= Available

BANKER'S ALGORITHM STEPS:
1. Calculate Need matrix
2. Find process where Need <= Work (available)
3. Allocate resources, mark complete
4. Release resources: Work = Work + Allocation
5. Repeat until all complete or stuck

SAMPLE INPUT:
Processes: 5, Resources: 3
Available: 3 3 2
Max and Allocation matrices
==============================================================================
*/

#include <iostream>
#include <iomanip>

using namespace std;

#define MAX_PROCESS 10
#define MAX_RESOURCE 10

int n, m;   // n=processes, m=resources
int available[MAX_RESOURCE];
int max_matrix[MAX_PROCESS][MAX_RESOURCE];
int allocation[MAX_PROCESS][MAX_RESOURCE];
int need[MAX_PROCESS][MAX_RESOURCE];

// Calculate Need matrix: Need = Max - Allocation
void calculateNeed() {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            need[i][j] = max_matrix[i][j] - allocation[i][j];
        }
    }
}

// Display matrix
void displayMatrix(int matrix[][MAX_RESOURCE], string name) {
    cout << "\n" << name << " Matrix:" << endl;
    for(int i = 0; i < n; i++) {
        cout << "P" << i << ": ";
        for(int j = 0; j < m; j++) {
            cout << setw(3) << matrix[i][j];
        }
        cout << endl;
    }
}

// Check if need <= work for process i
bool canAllocate(int process, int work[]) {
    for(int j = 0; j < m; j++) {
        if(need[process][j] > work[j])
            return false;
    }
    return true;
}

// Safety Algorithm - core of Banker's
bool isSafeState(int safeSeq[]) {
    int work[MAX_RESOURCE];
    bool finish[MAX_PROCESS] = {false};
    
    // Initialize work = available
    for(int i = 0; i < m; i++) {
        work[i] = available[i];
    }
    
    int count = 0;
    
    // Find safe sequence
    while(count < n) {
        bool found = false;
        
        for(int i = 0; i < n; i++) {
            if(!finish[i] && canAllocate(i, work)) {
                // Process i can complete
                cout << "\nP" << i << " can complete (Need <= Work)" << endl;
                cout << "  Work: ";
                for(int j = 0; j < m; j++) cout << work[j] << " ";
                cout << endl;
                
                // Release resources
                for(int j = 0; j < m; j++) {
                    work[j] += allocation[i][j];
                }
                
                cout << "  After P" << i << " completes, Work: ";
                for(int j = 0; j < m; j++) cout << work[j] << " ";
                cout << endl;
                
                safeSeq[count++] = i;
                finish[i] = true;
                found = true;
            }
        }
        
        if(!found) {
            // No process can proceed - UNSAFE
            cout << "\n❌ No process can proceed. System is UNSAFE!" << endl;
            cout << "Remaining processes: ";
            for(int i = 0; i < n; i++) {
                if(!finish[i]) cout << "P" << i << " ";
            }
            cout << endl;
            return false;
        }
    }
    
    return true;
}

int main() {
    cout << "========================================" << endl;
    cout << "  BANKER'S ALGORITHM - SAFETY CHECK" << endl;
    cout << "========================================\n" << endl;
    
    // Input
    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter number of resources: ";
    cin >> m;
    
    cout << "\nEnter Available resources:" << endl;
    for(int i = 0; i < m; i++) {
        cout << "Resource " << i << ": ";
        cin >> available[i];
    }
    
    cout << "\nEnter Max Matrix:" << endl;
    for(int i = 0; i < n; i++) {
        cout << "P" << i << ": ";
        for(int j = 0; j < m; j++) {
            cin >> max_matrix[i][j];
        }
    }
    
    cout << "\nEnter Allocation Matrix:" << endl;
    for(int i = 0; i < n; i++) {
        cout << "P" << i << ": ";
        for(int j = 0; j < m; j++) {
            cin >> allocation[i][j];
        }
    }
    
    // Calculate Need
    calculateNeed();
    
    // Display matrices
    cout << "\n========================================" << endl;
    cout << "  RESOURCE MATRICES" << endl;
    cout << "========================================" << endl;
    
    cout << "\nAvailable: ";
    for(int i = 0; i < m; i++) cout << available[i] << " ";
    cout << endl;
    
    displayMatrix(allocation, "Allocation");
    displayMatrix(max_matrix, "Max");
    displayMatrix(need, "Need");
    
    // Check safety
    cout << "\n========================================" << endl;
    cout << "  SAFETY ALGORITHM EXECUTION" << endl;
    cout << "========================================" << endl;
    
    int safeSeq[MAX_PROCESS];
    
    if(isSafeState(safeSeq)) {
        cout << "\n✓ System is in SAFE state!" << endl;
        cout << "\nSafe Sequence: ";
        for(int i = 0; i < n; i++) {
            cout << "P" << safeSeq[i];
            if(i < n-1) cout << " -> ";
        }
        cout << endl;
    }
    else {
        cout << "\n❌ System is in UNSAFE state!" << endl;
        cout << "Deadlock may occur!" << endl;
    }
    
    cout << "\n========================================" << endl;
    
    return 0;
}

/*
==============================================================================
EXAM NOTES:

1. BANKER'S ALGORITHM PURPOSE:
   - Deadlock avoidance algorithm
   - Ensures system never enters unsafe state
   - Used in resource allocation systems

2. KEY MATRICES:
   Available[m]: Available instances of each resource
   Max[n][m]: Maximum demand of each process
   Allocation[n][m]: Currently allocated to each process
   Need[n][m]: Remaining need = Max - Allocation

3. SAFE STATE:
   - System can allocate resources to all processes
   - in some order without deadlock
   - Safe sequence exists

4. UNSAFE STATE:
   - May lead to deadlock
   - No safe sequence exists
   - Doesn't guarantee deadlock, just possible

5. SAFETY ALGORITHM:
   Work = Available
   Finish[i] = false for all i
   While (unfinished processes exist):
       Find i where Finish[i]=false AND Need[i]<=Work
       If found:
           Work = Work + Allocation[i]
           Finish[i] = true
       Else:
           UNSAFE
   If all Finish[i]=true: SAFE

6. EXAMPLE CALCULATION:
   P0: Allocation=[0,1,0], Need=[7,4,3], Work=[3,3,2]
   Need NOT <= Work (7>3), skip
   P1: Allocation=[2,0,0], Need=[1,2,2], Work=[3,3,2]
   Need <= Work, can run!
   Work = [3,3,2] + [2,0,0] = [5,3,2]

7. DEADLOCK VS BANKER'S:
   - Deadlock Detection: Check if deadlock exists
   - Banker's: Prevent entering unsafe state
   - Prevention vs Detection

8. EXAM TIP:
   Always show:
   1. Need calculation
   2. Step-by-step safety check
   3. Work values at each step
   4. Safe sequence or why unsafe
==============================================================================
*/
