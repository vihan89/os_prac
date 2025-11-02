/*
2.1 Implement the C program to accept 'n' integers to be sorted. 
Main function creates child process using fork system call. 
Parent process sorts the integers using bubble sort and waits for child process using wait system call. 
Child process sorts the integers using insertion sort. 
Also demonstrate zombie and orphan states.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Bubble Sort function
void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for(i = 0; i < n-1; i++) {
        for(j = 0; j < n-i-1; j++) {
            if(arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// Insertion Sort function
void insertionSort(int arr[], int n) {
    int i, key, j;
    for(i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while(j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j = j - 1;
        }
        arr[j+1] = key;
    }
}

// Function to display array
void displayArray(int arr[], int n) {
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int n, i;
    
    printf("Enter number of integers: ");
    scanf("%d", &n);
    
    int arr[n], arr_copy[n];
    
    printf("Enter %d integers:\n", n);
    for(i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
        arr_copy[i] = arr[i];
    }
    
    printf("\nOriginal array: ");
    displayArray(arr, n);
    
    pid_t pid = fork();
    
    if(pid < 0) {
        printf("Fork failed!\n");
        return 1;
    }
    else if(pid == 0) {
        // Child process - Insertion Sort
        printf("\nChild Process (PID: %d)\n", getpid());
        printf("Sorting using Insertion Sort...\n");
        
        insertionSort(arr_copy, n);
        
        printf("Sorted array (Insertion Sort): ");
        displayArray(arr_copy, n);
        
        printf("Child process exiting...\n");
        exit(0);
    }
    else {
        // Parent process - Bubble Sort
        printf("\nParent Process (PID: %d)\n", getpid());
        
        // Demonstrate Zombie state
        printf("\n--- Demonstrating Zombie State ---\n");
        printf("Parent sleeping for 5 seconds (child becomes zombie)...\n");
        sleep(5);
        
        printf("Sorting using Bubble Sort...\n");
        bubbleSort(arr, n);
        
        printf("Sorted array (Bubble Sort): ");
        displayArray(arr, n);
        
        // Wait for child process
        wait(NULL);
        printf("Parent process: Child has completed.\n");
    }
    
    // Demonstrate Orphan state (uncomment to test)
    /*
    if(pid > 0) {
        printf("\n--- Demonstrating Orphan State ---\n");
        printf("Parent exiting immediately (child becomes orphan)...\n");
        exit(0);
    }
    else if(pid == 0) {
        sleep(3);
        printf("Child (PID: %d) is now orphan, adopted by init (PPID: %d)\n", 
               getpid(), getppid());
    }
    */
    
    return 0;
}
