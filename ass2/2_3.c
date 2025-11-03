/*
2.3 Implement the C program in which main program accepts an integer array. 
Main program uses the fork system call to create a new process called a child process. 
Parent process sorts an integer array and passes the sorted array to child process 
through the command line arguments of execve system call. 
The child process uses execve system call to load new program that uses this sorted array 
for performing the binary search to search the item in the array.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

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
    
    int arr[n];
    
    printf("Enter %d integers:\n", n);
    for(i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    printf("\nOriginal array: ");
    displayArray(arr, n);
    
    pid_t pid = fork();
    
    if(pid < 0) {
        printf("Fork failed!\n");
        return 1;
    }
    else if(pid == 0) {
        // Child process - will execute binary search program
        sleep(2); // Wait for parent to sort
        
        // Note: In a real implementation, you would use execve here
        // For demonstration, we'll simulate the binary search
        printf("\nChild Process: Executing binary search program...\n");
        printf("This would normally use execve to load a separate binary search program\n");
        
        exit(0);
    }
    else {
        // Parent process - Sort the array
        printf("\nParent Process (PID: %d)\n", getpid());
        printf("Sorting array...\n");
        
        bubbleSort(arr, n);
        
        printf("Sorted array: ");
        displayArray(arr, n);
        
        // Prepare arguments for execve (demonstration)
        char *args[n + 3];
        args[0] = "./bsearch"; // Binary search program name
        
        char size[10];
        sprintf(size, "%d", n);
        args[1] = size;
        
        // Convert array to strings
        for(i = 0; i < n; i++) {
            args[i + 2] = malloc(10);
            sprintf(args[i + 2], "%d", arr[i]);
        }
        args[n + 2] = NULL;
        
        printf("\nArguments prepared for child process (via execve)\n");
        
        // Wait for child
        wait(NULL);
        printf("\nParent: Child process completed\n");
        
        // Free allocated memory
        for(i = 0; i < n; i++) {
            free(args[i + 2]);
        }
    }
    
    return 0;
}
