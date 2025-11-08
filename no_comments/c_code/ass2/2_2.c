#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    printf("Sorted: ");
    for(int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int n, arr[100], key;
    char arg[100][20], *args[110];
    
    printf("Enter number of elements: ");
    scanf("%d", &n);
    printf("Enter elements:\n");
    for(int i = 0; i < n; i++) scanf("%d", &arr[i]);
    printf("Enter search key: ");
    scanf("%d", &key);
    
    bubbleSort(arr, n);
    
    pid_t pid = fork();
    
    if(pid == 0) {
        args[0] = (char*)"./2_2_search";
        sprintf(arg[0], "%d", n);
        args[1] = arg[0];
        for(int i = 0; i < n; i++) {
            sprintf(arg[i+1], "%d", arr[i]);
            args[i+2] = arg[i+1];
        }
        sprintf(arg[n+1], "%d", key);
        args[n+2] = arg[n+1];
        args[n+3] = NULL;
        
        execve("./2_2_search", args, NULL);
        perror("execve failed");
        return 1;
    } else {
        wait(NULL);
        printf("Parent: Search completed.\n");
    }
    
    return 0;
}
