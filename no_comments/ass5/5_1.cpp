

#include <iostream>
#include <iomanip>

using namespace std;

#define MAX_PROCESS 10
#define MAX_RESOURCE 10

int n, m;
int available[MAX_RESOURCE];
int max_matrix[MAX_PROCESS][MAX_RESOURCE];
int allocation[MAX_PROCESS][MAX_RESOURCE];
int need[MAX_PROCESS][MAX_RESOURCE];

void calculateNeed() {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            need[i][j] = max_matrix[i][j] - allocation[i][j];
        }
    }
}

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

bool canAllocate(int process, int work[]) {
    for(int j = 0; j < m; j++) {
        if(need[process][j] > work[j])
            return false;
    }
    return true;
}

bool isSafeState(int safeSeq[]) {
    int work[MAX_RESOURCE];
    bool finish[MAX_PROCESS] = {false};
    

    for(int i = 0; i < m; i++) {
        work[i] = available[i];
    }
    
    int count = 0;
    

    while(count < n) {
        bool found = false;
        
        for(int i = 0; i < n; i++) {
            if(!finish[i] && canAllocate(i, work)) {

                cout << "\nP" << i << " can complete (Need <= Work)" << endl;
                cout << "  Work: ";
                for(int j = 0; j < m; j++) cout << work[j] << " ";
                cout << endl;
                

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
    

    calculateNeed();
    

    cout << "\n========================================" << endl;
    cout << "  RESOURCE MATRICES" << endl;
    cout << "========================================" << endl;
    
    cout << "\nAvailable: ";
    for(int i = 0; i < m; i++) cout << available[i] << " ";
    cout << endl;
    
    displayMatrix(allocation, "Allocation");
    displayMatrix(max_matrix, "Max");
    displayMatrix(need, "Need");
    

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

