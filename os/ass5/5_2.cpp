/*
==============================================================================
ASSIGNMENT 5.2: BANKER'S ALGORITHM - WITH RESOURCE REQUEST
==============================================================================

PROBLEM:
- All features of 5.1
- Additionally handle resource request from process
- Check if request can be granted safely
- Update matrices if granted
- Display grant/reject with reason

COMPILATION:
g++ 5_2.cpp -o 5_2

EXECUTION:
./5_2

RESOURCE REQUEST ALGORITHM:
1. Check if Request <= Need (valid request)
2. Check if Request <= Available (resources available)
3. Pretend allocation (update Available, Allocation, Need)
4. Run safety algorithm
5. If safe: Grant request permanently
   If unsafe: Rollback and reject

SAMPLE SCENARIO:
Request from P1: [1, 0, 2]
Check: Request <= Need? Request <= Available?
Try allocation, check safety
==============================================================================
*/

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

// Calculate Need matrix
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

// Check if need <= work
bool canAllocate(int process, int work[]) {
    for(int j = 0; j < m; j++) {
        if(need[process][j] > work[j])
            return false;
    }
    return true;
}

// Safety Algorithm
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

// Handle resource request
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
    
    // Step 1: Check if Request <= Need
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
    
    // Step 2: Check if Request <= Available
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
    
    // Step 3: Pretend allocation
    cout << "\n--- Step 3: Pretend Allocation ---" << endl;
    
    // Save old state
    int old_available[MAX_RESOURCE];
    int old_allocation[MAX_RESOURCE];
    int old_need[MAX_RESOURCE];
    
    for(int i = 0; i < m; i++) {
        old_available[i] = available[i];
        old_allocation[i] = allocation[process][i];
        old_need[i] = need[process][i];
    }
    
    // Update state
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
    
    // Step 4: Check safety
    cout << "\n--- Step 4: Safety Check ---" << endl;
    int safeSeq[MAX_PROCESS];
    
    if(isSafeState(safeSeq, true)) {
        // Safe - grant request
        cout << "\n✓ System remains in SAFE state!" << endl;
        cout << "Safe Sequence: ";
        for(int i = 0; i < n; i++) {
            cout << "P" << safeSeq[i];
            if(i < n-1) cout << " -> ";
        }
        cout << endl;
        
        cout << "\n✅ Request GRANTED!" << endl;
        cout << "Resources allocated to P" << process << endl;
        
        // Display updated matrices
        displayMatrix(allocation, "Updated Allocation");
        displayMatrix(need, "Updated Need");
        cout << "\nUpdated Available: ";
        for(int i = 0; i < m; i++) cout << available[i] << " ";
        cout << endl;
    }
    else {
        // Unsafe - rollback
        cout << "\n❌ Request DENIED!" << endl;
        cout << "Reason: Granting request leads to UNSAFE state!" << endl;
        cout << "System may enter deadlock!" << endl;
        
        // Restore old state
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
    
    // Display initial state
    cout << "\n========================================" << endl;
    cout << "  INITIAL STATE" << endl;
    cout << "========================================" << endl;
    
    cout << "\nAvailable: ";
    for(int i = 0; i < m; i++) cout << available[i] << " ";
    cout << endl;
    
    displayMatrix(allocation, "Allocation");
    displayMatrix(max_matrix, "Max");
    displayMatrix(need, "Need");
    
    // Check initial safety
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
    
    // Handle requests in loop
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

/*
==============================================================================
EXAM NOTES:

1. RESOURCE REQUEST ALGORITHM:
   Step 1: If Request[i] > Need[i]: ERROR (invalid)
   Step 2: If Request[i] > Available[i]: WAIT
   Step 3: Pretend allocation:
           Available = Available - Request
           Allocation = Allocation + Request
           Need = Need - Request
   Step 4: Run safety algorithm
           If safe: Grant (keep changes)
           If unsafe: Deny (rollback)

2. THREE POSSIBLE OUTCOMES:
   a) GRANTED: Request valid, resources available, state safe
   b) DENIED (Invalid): Request > Need (exceeded max claim)
   c) DENIED (Wait): Request > Available (resources insufficient)
   d) DENIED (Unsafe): Granting leads to unsafe state

3. ROLLBACK MECHANISM:
   - Save old Available, Allocation, Need
   - Make pretend changes
   - If unsafe, restore old values
   - Critical for maintaining safety

4. EXAMPLE:
   Current: Available=[3,3,2], Allocation[P1]=[2,0,0], Need[P1]=[1,2,2]
   Request from P1: [1,0,2]
   
   Check 1: [1,0,2] <= [1,2,2]? YES
   Check 2: [1,0,2] <= [3,3,2]? YES
   Pretend: Available=[2,3,0], Allocation[P1]=[3,0,2], Need[P1]=[0,2,0]
   Safety check... if SAFE: grant, if UNSAFE: deny

5. WHY PRETEND ALLOCATION?
   - Can't just give resources and check
   - If unsafe, can't take back (process may have used)
   - Must simulate before actual allocation

6. MULTIPLE REQUESTS:
   - Handle one request at a time
   - After granting, state changes
   - Next request uses updated state

7. EXAM MISTAKES TO AVOID:
   - Not checking Request <= Need
   - Not rolling back on unsafe
   - Forgetting to update all 3 matrices
   - Not showing step-by-step reasoning

8. REAL-WORLD APPLICATION:
   - Operating system resource allocation
   - Database transaction management
   - Distributed systems
   - Cloud resource provisioning

9. TIME COMPLEXITY:
   - Safety check: O(n² × m)
   - n = processes, m = resources
   - For each process, check m resources
   - Repeat n times

10. EXAM TIP:
    Always show:
    1. Request validation (2 checks)
    2. Pretend allocation (3 matrix updates)
    3. Safety algorithm execution
    4. Grant/Deny decision with reason
    5. Rollback if denied
==============================================================================
*/
