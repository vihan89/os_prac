

#include <iostream>
#include <cstdlib>

using namespace std;

#define MAX 100

int binarySearch(int arr[], int n, int key) {
    int left = 0;
    int right = n - 1;
    
    cout << "\n[BINARY SEARCH] Searching for " << key << " in sorted array..." << endl;
    
    while(left <= right) {
        int mid = left + (right - left) / 2;
        
        cout << "  Checking range [" << left << ", " << right 
             << "], mid = " << mid << ", arr[mid] = " << arr[mid] << endl;
        

        if(arr[mid] == key) {
            return mid;
        }
        

        if(arr[mid] < key) {
            left = mid + 1;
        }

        else {
            right = mid - 1;
        }
    }
    
    return -1;
}

int main(int argc, char* argv[]) {
    cout << "\n========================================" << endl;
    cout << "  BINARY SEARCH PROGRAM" << endl;
    cout << "  (Executed via execve)" << endl;
    cout << "========================================" << endl;
    

    
    cout << "\n[CHILD PROCESS] Received " << argc << " arguments" << endl;
    

    if(argc < 4) {
        cout << "Error: Insufficient arguments!" << endl;
        cout << "Usage: " << argv[0] << " <n> <arr[0]> ... <arr[n-1]> <key>" << endl;
        return 1;
    }
    

    int n = atoi(argv[1]);
    
    if(n <= 0 || n > MAX) {
        cout << "Error: Invalid number of elements!" << endl;
        return 1;
    }
    

    if(argc != n + 3) {
        cout << "Error: Argument count mismatch!" << endl;
        cout << "Expected " << n + 3 << " arguments, got " << argc << endl;
        return 1;
    }
    

    int arr[MAX];
    cout << "\n[CHILD] Received sorted array: ";
    for(int i = 0; i < n; i++) {
        arr[i] = atoi(argv[i + 2]);
        cout << arr[i] << " ";
    }
    cout << endl;
    

    int searchKey = atoi(argv[n + 2]);
    cout << "[CHILD] Search key: " << searchKey << endl;
    

    
    int result = binarySearch(arr, n, searchKey);
    

    
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

