/*
==============================================================================
ASSIGNMENT 2.2: FORK WITH EXECVE AND BINARY SEARCH
==============================================================================

PROBLEM:
- Parent sorts array using bubble sort
- Pass sorted array to child via execve() command line arguments
- Create separate child program that performs binary search
- Handle string to int conversion for argv

COMPILATION:
g++ 2_2.cpp -o 2_2
g++ 2_2_search.cpp -o 2_2_search

EXECUTION:
./2_2

KEY CONCEPTS TO REMEMBER:
1. execve() replaces current process image with new program
2. execve(path, argv, envp) - argv[0] is program name
3. execve() only returns if error occurs (-1)
4. Command line args passed as char* array (strings)
5. Use atoi() to convert string to integer

SAMPLE RUN:
Enter number of elements: 5
Enter 5 integers: 64 34 25 12 22
Sorted array: 12 22 25 34 64
Enter element to search: 25
Element 25 found at position 2
==============================================================================
*/

#include <iostream>
#include <unistd.h>     // fork(), execve()
#include <sys/wait.h>   // wait()
#include <cstdlib>      // exit(), atoi()
#include <cstring>      // strcpy()

using namespace std;

#define MAX 100

// ============================================================================
// BUBBLE SORT ALGORITHM
// Time Complexity: O(n^2)
// Sorts array in ascending order
// ============================================================================
void bubbleSort(int arr[], int n) {
    cout << "\n[PARENT] Sorting array using Bubble Sort..." << endl;
    
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(arr[j] > arr[j+1]) {
                // Swap arr[j] and arr[j+1]
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
    
    cout << "[PARENT] Sorted array: ";
    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// ============================================================================
// CONVERT INTEGER TO STRING
// Helper function for execve() argument passing
// ============================================================================
void intToString(int num, char* str) {
    sprintf(str, "%d", num);
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    int arr[MAX], n, searchKey;
    
    cout << "========================================" << endl;
    cout << "  FORK WITH EXECVE" << endl;
    cout << "  (Sorted Array + Binary Search)" << endl;
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
    
    // Input search key
    cout << "Enter element to search: ";
    cin >> searchKey;
    
    // Parent sorts the array
    bubbleSort(arr, n);
    
    // ========================================================================
    // FORK TO CREATE CHILD PROCESS
    // ========================================================================
    pid_t pid = fork();
    
    if(pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if(pid == 0) {
        // ====================================================================
        // CHILD PROCESS - EXECVE TO BINARY SEARCH PROGRAM
        // ====================================================================
        
        cout << "\n[CHILD] Preparing to execute binary search program..." << endl;
        
        // Prepare arguments for execve()
        // Format: ./2_2_search n arr[0] arr[1] ... arr[n-1] searchKey
        
        char* args[MAX + 10];  // Program name + n + array elements + key + NULL
        char argStrings[MAX + 10][20];  // Storage for string arguments
        
        // args[0] = program name
        args[0] = (char*)"./2_2_search";
        
        // args[1] = number of elements
        intToString(n, argStrings[0]);
        args[1] = argStrings[0];
        
        // args[2] to args[n+1] = array elements
        for(int i = 0; i < n; i++) {
            intToString(arr[i], argStrings[i+1]);
            args[i+2] = argStrings[i+1];
        }
        
        // args[n+2] = search key
        intToString(searchKey, argStrings[n+1]);
        args[n+2] = argStrings[n+1];
        
        // args[n+3] = NULL (terminate argument list)
        args[n+3] = NULL;
        
        // Display what we're passing
        cout << "[CHILD] Executing: ./2_2_search ";
        for(int i = 1; i < n+3; i++) {
            cout << args[i] << " ";
        }
        cout << endl;
        
        // Execute the binary search program
        // execve() replaces current process image
        execve("./2_2_search", args, NULL);
        
        // If execve() returns, it failed
        perror("[CHILD] execve failed");
        exit(1);
    }
    else {
        // ====================================================================
        // PARENT PROCESS - WAIT FOR CHILD
        // ====================================================================
        
        cout << "[PARENT] Waiting for child to complete binary search..." << endl;
        
        int status;
        wait(&status);  // Wait for child to finish
        
        if(WIFEXITED(status)) {
            cout << "\n[PARENT] Child completed with exit status: " 
                 << WEXITSTATUS(status) << endl;
        }
        
        cout << "[PARENT] Program completed successfully!" << endl;
    }
    
    return 0;
}

/*
==============================================================================
IMPORTANT NOTES FOR EXAM:

1. EXECVE() FUNCTION:
   - Syntax: execve(const char *path, char *const argv[], char *const envp[])
   - Replaces current process with new program
   - Does NOT create new process (unlike fork)
   - Only returns if error occurs
   - argv[0] should be program name
   - argv array must end with NULL

2. FORK + EXECVE PATTERN:
   - fork() creates new process
   - Child calls execve() to run different program
   - Parent waits for child completion
   - Common pattern in shell implementation

3. COMMAND LINE ARGUMENTS:
   - argc: argument count
   - argv: argument vector (array of strings)
   - argv[0]: program name
   - argv[1] to argv[argc-1]: actual arguments
   - Use atoi() to convert string to int

4. PROCESS REPLACEMENT:
   - execve() replaces code, data, heap, stack
   - PID remains same
   - Open file descriptors may be preserved
   - Parent-child relationship maintained

5. BUBBLE SORT KEY POINTS:
   - Compare adjacent elements
   - Swap if in wrong order
   - After each pass, largest element "bubbles" to end
   - Time: O(n²), Space: O(1)
==============================================================================
*/
