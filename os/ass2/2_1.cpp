/*
==============================================================================
ASSIGNMENT 2.1: FORK WITH SORTING (ZOMBIE & ORPHAN DEMONSTRATION)
==============================================================================

PROBLEM:
- Accept n integers in array
- Use fork() to create child process
- Parent: bubble sort, wait() for child
- Child: insertion sort
- Demonstrate zombie state (child exits, parent delays wait())
- Demonstrate orphan state (parent exits before child)

COMPILATION:
g++ 2_1.cpp -o 2_1

EXECUTION:
./2_1

KEY CONCEPTS TO REMEMBER:
1. fork() returns: 0 to child, child's PID to parent, -1 on error
2. Zombie: Child exits but parent hasn't called wait() yet
3. Orphan: Parent exits before child, child adopted by init process
4. wait(NULL) makes parent wait for child termination

SAMPLE RUN:
Enter number of elements: 5
Enter 5 integers: 64 34 25 12 22
==============================================================================
*/

#include <iostream>
#include <unistd.h>     // fork(), getpid(), getppid(), sleep()
#include <sys/wait.h>   // wait()
#include <cstdlib>      // exit()

using namespace std;

#define MAX 100

// ============================================================================
// BUBBLE SORT ALGORITHM
// Time Complexity: O(n^2)
// Used by PARENT process
// ============================================================================
void bubbleSort(int arr[], int n) {
    cout << "\n[PARENT - PID: " << getpid() << "] Starting Bubble Sort..." << endl;
    
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(arr[j] > arr[j+1]) {
                // Swap elements
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
    
    cout << "[PARENT] Bubble Sort Result: ";
    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// ============================================================================
// INSERTION SORT ALGORITHM
// Time Complexity: O(n^2)
// Used by CHILD process
// ============================================================================
void insertionSort(int arr[], int n) {
    cout << "\n[CHILD - PID: " << getpid() << ", Parent PID: " << getppid() 
         << "] Starting Insertion Sort..." << endl;
    
    for(int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        // Move elements greater than key one position ahead
        while(j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
    
    cout << "[CHILD] Insertion Sort Result: ";
    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// ============================================================================
// DEMONSTRATE ZOMBIE STATE
// Child exits first, parent delays calling wait()
// ============================================================================
void demonstrateZombie(int arr[], int n) {
    cout << "\n========================================" << endl;
    cout << "  DEMONSTRATING ZOMBIE STATE" << endl;
    cout << "========================================" << endl;
    
    pid_t pid = fork();
    
    if(pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if(pid == 0) {
        // CHILD PROCESS
        insertionSort(arr, n);
        cout << "[CHILD] Exiting now..." << endl;
        exit(0);  // Child exits immediately
    }
    else {
        // PARENT PROCESS
        cout << "[PARENT] Child PID: " << pid << endl;
        cout << "[PARENT] Sleeping for 5 seconds (child becomes ZOMBIE)..." << endl;
        sleep(5);  // Parent delays, child is zombie during this time
        
        cout << "[PARENT] Now calling wait() to reap zombie child..." << endl;
        wait(NULL);  // Reap the zombie
        cout << "[PARENT] Zombie child reaped successfully!" << endl;
        
        bubbleSort(arr, n);
    }
}

// ============================================================================
// DEMONSTRATE ORPHAN STATE
// Parent exits first, child becomes orphan (adopted by init)
// ============================================================================
void demonstrateOrphan(int arr[], int n) {
    cout << "\n========================================" << endl;
    cout << "  DEMONSTRATING ORPHAN STATE" << endl;
    cout << "========================================" << endl;
    
    pid_t pid = fork();
    
    if(pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if(pid == 0) {
        // CHILD PROCESS
        cout << "[CHILD] Initial Parent PID: " << getppid() << endl;
        sleep(3);  // Sleep to let parent exit first
        cout << "[CHILD] After parent exit, new Parent PID: " << getppid() 
             << " (adopted by init)" << endl;
        insertionSort(arr, n);
        exit(0);
    }
    else {
        // PARENT PROCESS
        cout << "[PARENT] My PID: " << getpid() << ", Child PID: " << pid << endl;
        cout << "[PARENT] Exiting immediately (child becomes ORPHAN)..." << endl;
        exit(0);  // Parent exits, child becomes orphan
    }
}

// ============================================================================
// NORMAL EXECUTION
// Both processes work normally with proper wait()
// ============================================================================
void normalExecution(int arr[], int n) {
    cout << "\n========================================" << endl;
    cout << "  NORMAL EXECUTION WITH WAIT()" << endl;
    cout << "========================================" << endl;
    
    pid_t pid = fork();
    
    if(pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if(pid == 0) {
        // CHILD PROCESS
        insertionSort(arr, n);
        exit(0);
    }
    else {
        // PARENT PROCESS
        cout << "[PARENT] Waiting for child to complete..." << endl;
        wait(NULL);  // Wait for child to finish
        cout << "[PARENT] Child completed. Now starting parent work..." << endl;
        bubbleSort(arr, n);
    }
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    int arr[MAX], n, choice;
    
    cout << "========================================" << endl;
    cout << "  FORK WITH SORTING" << endl;
    cout << "  (Zombie & Orphan Demonstration)" << endl;
    cout << "========================================" << endl;
    
    // Input array
    cout << "\nEnter number of elements (max " << MAX << "): ";
    cin >> n;
    
    if(n <= 0 || n > MAX) {
        cout << "Invalid input!" << endl;
        return 1;
    }
    
    cout << "Enter " << n << " integers: ";
    for(int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    // Display menu
    cout << "\n========================================" << endl;
    cout << "           MENU" << endl;
    cout << "========================================" << endl;
    cout << "1. Normal Execution (with wait())" << endl;
    cout << "2. Demonstrate Zombie State" << endl;
    cout << "3. Demonstrate Orphan State" << endl;
    cout << "========================================" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    
    // Create copy of array for each demonstration
    int arrCopy[MAX];
    for(int i = 0; i < n; i++) {
        arrCopy[i] = arr[i];
    }
    
    switch(choice) {
        case 1:
            normalExecution(arrCopy, n);
            break;
        case 2:
            demonstrateZombie(arrCopy, n);
            break;
        case 3:
            demonstrateOrphan(arrCopy, n);
            cout << "\nNote: Parent exited. If you see orphan child output, ";
            cout << "it was adopted by init process." << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
    }
    
    return 0;
}

/*
==============================================================================
IMPORTANT NOTES FOR EXAM:

1. ZOMBIE PROCESS:
   - Child exits but parent hasn't called wait()
   - Entry remains in process table
   - Shows as <defunct> in process list
   - Reaped when parent calls wait()

2. ORPHAN PROCESS:
   - Parent exits before child
   - Child adopted by init process (PID 1)
   - Init automatically reaps orphan children
   - getppid() will return 1 after adoption

3. FORK RETURN VALUES:
   - Returns 0 in child process
   - Returns child's PID in parent
   - Returns -1 on error

4. WAIT FUNCTION:
   - Makes parent wait for child termination
   - Prevents zombie processes
   - Returns PID of terminated child
   
5. SORTING ALGORITHMS:
   - Bubble Sort: Compare adjacent, swap if needed, O(n²)
   - Insertion Sort: Insert each element in sorted position, O(n²)
==============================================================================
*/
