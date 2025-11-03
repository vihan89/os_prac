/*
==============================================================================
ASSIGNMENT 2.2: BINARY SEARCH PROGRAM (CALLED BY EXECVE)
==============================================================================

PROBLEM:
- Receives sorted array and search key via command line arguments
- Performs binary search on the array
- Displays result (found/not found with position)

COMPILATION:
g++ 2_2_search.cpp -o 2_2_search

NOTE: This program is called by 2_2.cpp via execve()

COMMAND LINE FORMAT:
./2_2_search <n> <arr[0]> <arr[1]> ... <arr[n-1]> <searchKey>

EXAMPLE:
./2_2_search 5 12 22 25 34 64 25

KEY CONCEPTS TO REMEMBER:
1. argc: number of command line arguments
2. argv[0]: program name
3. argv[1]: number of elements
4. argv[2] to argv[n+1]: array elements
5. argv[n+2]: search key
6. atoi(): converts string to integer

BINARY SEARCH:
- Works only on SORTED arrays
- Time Complexity: O(log n)
- Divides search space in half each time
==============================================================================
*/

#include <iostream>
#include <cstdlib>  // atoi(), exit()

using namespace std;

#define MAX 100

// ============================================================================
// BINARY SEARCH ALGORITHM
// Time Complexity: O(log n)
// Works on SORTED array only
// ============================================================================
int binarySearch(int arr[], int n, int key) {
    int left = 0;
    int right = n - 1;
    
    cout << "\n[BINARY SEARCH] Searching for " << key << " in sorted array..." << endl;
    
    while(left <= right) {
        int mid = left + (right - left) / 2;  // Avoid overflow
        
        cout << "  Checking range [" << left << ", " << right 
             << "], mid = " << mid << ", arr[mid] = " << arr[mid] << endl;
        
        // Check if key is at mid
        if(arr[mid] == key) {
            return mid;  // Element found
        }
        
        // If key is greater, ignore left half
        if(arr[mid] < key) {
            left = mid + 1;
        }
        // If key is smaller, ignore right half
        else {
            right = mid - 1;
        }
    }
    
    return -1;  // Element not found
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main(int argc, char* argv[]) {
    cout << "\n========================================" << endl;
    cout << "  BINARY SEARCH PROGRAM" << endl;
    cout << "  (Executed via execve)" << endl;
    cout << "========================================" << endl;
    
    // ========================================================================
    // PARSE COMMAND LINE ARGUMENTS
    // ========================================================================
    
    cout << "\n[CHILD PROCESS] Received " << argc << " arguments" << endl;
    
    // Minimum arguments: program_name + n + at_least_1_element + key
    if(argc < 4) {
        cout << "Error: Insufficient arguments!" << endl;
        cout << "Usage: " << argv[0] << " <n> <arr[0]> ... <arr[n-1]> <key>" << endl;
        return 1;
    }
    
    // Parse number of elements
    int n = atoi(argv[1]);
    
    if(n <= 0 || n > MAX) {
        cout << "Error: Invalid number of elements!" << endl;
        return 1;
    }
    
    // Check if we have enough arguments
    if(argc != n + 3) {  // program_name + n + n_elements + key
        cout << "Error: Argument count mismatch!" << endl;
        cout << "Expected " << n + 3 << " arguments, got " << argc << endl;
        return 1;
    }
    
    // Parse array elements
    int arr[MAX];
    cout << "\n[CHILD] Received sorted array: ";
    for(int i = 0; i < n; i++) {
        arr[i] = atoi(argv[i + 2]);  // argv[2] to argv[n+1]
        cout << arr[i] << " ";
    }
    cout << endl;
    
    // Parse search key
    int searchKey = atoi(argv[n + 2]);
    cout << "[CHILD] Search key: " << searchKey << endl;
    
    // ========================================================================
    // PERFORM BINARY SEARCH
    // ========================================================================
    
    int result = binarySearch(arr, n, searchKey);
    
    // ========================================================================
    // DISPLAY RESULT
    // ========================================================================
    
    cout << "\n========================================" << endl;
    cout << "         SEARCH RESULT" << endl;
    cout << "========================================" << endl;
    
    if(result != -1) {
        cout << "✓ Element " << searchKey << " FOUND at index " << result << endl;
        cout << "  Position: " << (result + 1) << " (1-based indexing)" << endl;
    }
    else {
        cout << "✗ Element " << searchKey << " NOT FOUND in array" << endl;
    }
    
    cout << "========================================" << endl;
    
    return 0;
}

/*
==============================================================================
IMPORTANT NOTES FOR EXAM:

1. BINARY SEARCH ALGORITHM:
   - Prerequisite: Array must be SORTED
   - Time Complexity: O(log n) - very efficient
   - Space Complexity: O(1) - iterative version
   
2. ALGORITHM STEPS:
   a) Set left = 0, right = n-1
   b) While left <= right:
      - Calculate mid = (left + right) / 2
      - If arr[mid] == key, return mid
      - If arr[mid] < key, search right half (left = mid + 1)
      - If arr[mid] > key, search left half (right = mid - 1)
   c) If loop ends, element not found

3. COMMAND LINE ARGUMENTS:
   - argc: count of arguments (including program name)
   - argv[]: array of string pointers
   - argv[0]: always program name
   - Use atoi() to convert string to int
   - Use atof() to convert string to float

4. ATOI() FUNCTION:
   - Converts string to integer
   - Returns 0 if conversion fails
   - Stops at first non-digit character
   - Example: atoi("123") returns 123

5. WHY BINARY SEARCH IS EFFICIENT:
   - Eliminates half the search space each iteration
   - For n=1000, max iterations = log₂(1000) ≈ 10
   - For n=1,000,000, max iterations ≈ 20
   
6. COMPARISON WITH LINEAR SEARCH:
   - Linear Search: O(n) - check each element
   - Binary Search: O(log n) - divide and conquer
   - Binary Search requires sorted array
==============================================================================
*/
