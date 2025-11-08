#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void bubbleSort(int arr[], int n) {
    for(int i = 0; i < n-1; i++)
        for(int j = 0; j < n-i-1; j++)
            if(arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
    printf("Parent sorted: ");
    for(int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

void insertionSort(int arr[], int n) {
    for(int i = 1; i < n; i++) {
        int key = arr[i], j = i - 1;
        while(j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
    printf("Child sorted: ");
    for(int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int n, arr[100];
    
    printf("Enter number of elements: ");
    scanf("%d", &n);
    printf("Enter elements:\n");
    for(int i = 0; i < n; i++) scanf("%d", &arr[i]);
    
    printf("\n1. Normal fork\n2. Zombie demo\n3. Orphan demo\nChoice: ");
    int choice;
    scanf("%d", &choice);
    
    pid_t pid = fork();
    
    if(pid == 0) {
        if(choice == 3) {
            printf("Child: parent was %d", getppid());
            sleep(3);
            printf(", now %d (orphan!)\n", getppid());
        }
        insertionSort(arr, n);
    } else {
        if(choice == 2) {
            printf("Parent PID: %d, Child PID: %d (zombie)\n", getpid(), pid);
            printf("Run: ps aux | grep Z\n");
            while(1) sleep(1);
        }
        if(choice != 3) wait(NULL);
        bubbleSort(arr, n);
    }
    
    return 0;
}
