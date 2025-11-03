

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

bool isSafeState(int safeSeq[], bool verbose = true) {
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
                if(verbose) {
                    cout << "\n  P" << i << " can complete (Need <= Work)" << endl;
                    cout << "    Work before: ";
                    for(int j = 0; j < m; j++) cout << work[j] << " ";
                    cout << endl;
                }
                
                for(int j = 0; j < m; j++) {
                    work[j] += allocation[i][j];
                }
                
                if(verbose) {
                    cout << "    Work after: ";
                    for(int j = 0; j < m; j++) cout << work[j] << " ";
                    cout << endl;
                }
                
                safeSeq[count++] = i;
                finish[i] = true;
                found = true;
            }
        }
        
        if(!found) {
            if(verbose) {
                cout << "\n  ❌ No process can proceed - UNSAFE!" << endl;
            }
            return false;
        }
    }
    
    return true;
}

void handleRequest() {
    int process;
    int request[MAX_RESOURCE];
    
    cout << "\n========================================" << endl;
    cout << "  RESOURCE REQUEST" << endl;
    cout << "========================================" << endl;
    
    cout << "\nEnter process number (0-" << n-1 << "): ";
    cin >> process;
    
    if(process < 0 || process >= n) {
        cout << "❌ Invalid process number!" << endl;
        return;
    }
    
    cout << "Enter resource request:" << endl;
    for(int i = 0; i < m; i++) {
        cout << "Resource " << i << ": ";
        cin >> request[i];
    }
    
    cout << "\nRequest from P" << process << ": ";
    for(int i = 0; i < m; i++) cout << request[i] << " ";
    cout << endl;
    

    cout << "\n--- Step 1: Check Request <= Need ---" << endl;
    cout << "Need for P" << process << ": ";
    for(int i = 0; i < m; i++) cout << need[process][i] << " ";
    cout << endl;
    
    for(int i = 0; i < m; i++) {
        if(request[i] > need[process][i]) {
            cout << "❌ Request DENIED!" << endl;
            cout << "Reason: Request[" << i << "]=" << request[i] 
                 << " > Need[" << i << "]=" << need[process][i] << endl;
            cout << "Process has exceeded maximum claim!" << endl;
            return;
        }
    }
    cout << "✓ Request <= Need (Valid request)" << endl;
    

    cout << "\n--- Step 2: Check Request <= Available ---" << endl;
    cout << "Available: ";
    for(int i = 0; i < m; i++) cout << available[i] << " ";
    cout << endl;
    
    for(int i = 0; i < m; i++) {
        if(request[i] > available[i]) {
            cout << "❌ Request DENIED!" << endl;
            cout << "Reason: Request[" << i << "]=" << request[i] 
                 << " > Available[" << i << "]=" << available[i] << endl;
            cout << "Resources not available, P" << process << " must wait!" << endl;
            return;
        }
    }
    cout << "✓ Request <= Available (Resources available)" << endl;
    

    cout << "\n--- Step 3: Pretend Allocation ---" << endl;
    

    int old_available[MAX_RESOURCE];
    int old_allocation[MAX_RESOURCE];
    int old_need[MAX_RESOURCE];
    
    for(int i = 0; i < m; i++) {
        old_available[i] = available[i];
        old_allocation[i] = allocation[process][i];
        old_need[i] = need[process][i];
    }
    

    for(int i = 0; i < m; i++) {
        available[i] -= request[i];
        allocation[process][i] += request[i];
        need[process][i] -= request[i];
    }
    
    cout << "New Available: ";
    for(int i = 0; i < m; i++) cout << available[i] << " ";
    cout << endl;
    cout << "New Allocation[P" << process << "]: ";
    for(int i = 0; i < m; i++) cout << allocation[process][i] << " ";
    cout << endl;
    cout << "New Need[P" << process << "]: ";
    for(int i = 0; i < m; i++) cout << need[process][i] << " ";
    cout << endl;
    

    cout << "\n--- Step 4: Safety Check ---" << endl;
    int safeSeq[MAX_PROCESS];
    
    if(isSafeState(safeSeq, true)) {

        cout << "\n✓ System remains in SAFE state!" << endl;
        cout << "Safe Sequence: ";
        for(int i = 0; i < n; i++) {
            cout << "P" << safeSeq[i];
            if(i < n-1) cout << " -> ";
        }
        cout << endl;
        
        cout << "\n✅ Request GRANTED!" << endl;
        cout << "Resources allocated to P" << process << endl;
        

        displayMatrix(allocation, "Updated Allocation");
        displayMatrix(need, "Updated Need");
        cout << "\nUpdated Available: ";
        for(int i = 0; i < m; i++) cout << available[i] << " ";
        cout << endl;
    }
    else {

        cout << "\n❌ Request DENIED!" << endl;
        cout << "Reason: Granting request leads to UNSAFE state!" << endl;
        cout << "System may enter deadlock!" << endl;
        

        for(int i = 0; i < m; i++) {
            available[i] = old_available[i];
            allocation[process][i] = old_allocation[i];
            need[process][i] = old_need[i];
        }
        
        cout << "\nState rolled back to previous safe state." << endl;
    }
}

int main() {
    cout << "========================================" << endl;
    cout << "  BANKER'S ALGORITHM WITH REQUESTS" << endl;
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
    cout << "  INITIAL STATE" << endl;
    cout << "========================================" << endl;
    
    cout << "\nAvailable: ";
    for(int i = 0; i < m; i++) cout << available[i] << " ";
    cout << endl;
    
    displayMatrix(allocation, "Allocation");
    displayMatrix(max_matrix, "Max");
    displayMatrix(need, "Need");
    

    cout << "\n========================================" << endl;
    cout << "  INITIAL SAFETY CHECK" << endl;
    cout << "========================================" << endl;
    
    int safeSeq[MAX_PROCESS];
    
    if(isSafeState(safeSeq, true)) {
        cout << "\n✓ Initial state is SAFE!" << endl;
        cout << "\nSafe Sequence: ";
        for(int i = 0; i < n; i++) {
            cout << "P" << safeSeq[i];
            if(i < n-1) cout << " -> ";
        }
        cout << endl;
    }
    else {
        cout << "\n❌ Initial state is UNSAFE!" << endl;
        cout << "Cannot proceed with resource requests!" << endl;
        return 0;
    }
    

    char choice;
    do {
        handleRequest();
        
        cout << "\nHandle another request? (y/n): ";
        cin >> choice;
    } while(choice == 'y' || choice == 'Y');
    
    cout << "\n========================================" << endl;
    cout << "  PROGRAM ENDED" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

