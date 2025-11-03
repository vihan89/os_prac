/*
2.2 Implement the C program in which main program accepts the integers to be sorted. 
Main program uses the fork system call to create a new process called a child process. 
Parent process sorts the integers using merge sort and waits for child process using wait system call 
to sort the integers using quick sort. 
Also demonstrate zombie and orphan states.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Merge function for merge sort
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    
    int L[n1], R[n2];
    
    for(i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for(j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    
    i = 0;
    j = 0;
    k = l;
    
    while(i < n1 && j < n2) {
        if(L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while(i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while(j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Merge Sort function
void mergeSort(int arr[], int l, int r) {
    if(l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Partition function for quick sort
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    
    for(int j = low; j <= high - 1; j++) {
        if(arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

// Quick Sort function
void quickSort(int arr[], int low, int high) {
    if(low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
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
        // Child process - Quick Sort
        printf("\nChild Process (PID: %d)\n", getpid());
        printf("Sorting using Quick Sort...\n");
        
        quickSort(arr_copy, 0, n - 1);
        
        printf("Sorted array (Quick Sort): ");
        displayArray(arr_copy, n);
        
        printf("Child process exiting...\n");
        exit(0);
    }
    else {
        // Parent process - Merge Sort
        printf("\nParent Process (PID: %d)\n", getpid());
        
        // Demonstrate Zombie state
        printf("\n--- Demonstrating Zombie State ---\n");
        printf("Parent sleeping for 5 seconds (child becomes zombie)...\n");
        sleep(5);
        
        printf("Sorting using Merge Sort...\n");
        mergeSort(arr, 0, n - 1);
        
        printf("Sorted array (Merge Sort): ");
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
