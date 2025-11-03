

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring>

using namespace std;

#define MAX 100

void bubbleSort(int arr[], int n) {
    cout << "\n[PARENT] Sorting array using Bubble Sort..." << endl;
    
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(arr[j] > arr[j+1]) {

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

void intToString(int num, char* str) {
    sprintf(str, "%d", num);
}

int main() {
    int arr[MAX], n, searchKey;
    
    cout << "========================================" << endl;
    cout << "  FORK WITH EXECVE" << endl;
    cout << "  (Sorted Array + Binary Search)" << endl;
    cout << "========================================" << endl;
    

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
    

    cout << "Enter element to search: ";
    cin >> searchKey;
    

    bubbleSort(arr, n);
    

    pid_t pid = fork();
    
    if(pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if(pid == 0) {

        
        cout << "\n[CHILD] Preparing to execute binary search program..." << endl;
        

        
        char* args[MAX + 10];
        char argStrings[MAX + 10][20];
        

        args[0] = (char*)"./2_2_search";
        

        intToString(n, argStrings[0]);
        args[1] = argStrings[0];
        

        for(int i = 0; i < n; i++) {
            intToString(arr[i], argStrings[i+1]);
            args[i+2] = argStrings[i+1];
        }
        

        intToString(searchKey, argStrings[n+1]);
        args[n+2] = argStrings[n+1];
        

        args[n+3] = NULL;
        

        cout << "[CHILD] Executing: ./2_2_search ";
        for(int i = 1; i < n+3; i++) {
            cout << args[i] << " ";
        }
        cout << endl;
        

        execve("./2_2_search", args, NULL);
        

        perror("[CHILD] execve failed");
        exit(1);
    }
    else {

        
        cout << "[PARENT] Waiting for child to complete binary search..." << endl;
        
        int status;
        wait(&status);
        
        if(WIFEXITED(status)) {
            cout << "\n[PARENT] Child completed with exit status: " 
                 << WEXITSTATUS(status) << endl;
        }
        
        cout << "[PARENT] Program completed successfully!" << endl;
    }
    
    return 0;
}

