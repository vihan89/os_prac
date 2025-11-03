/*
2.3 - Binary Search Program (Child Program)
This program is called by 2_3.c using execve.
It receives sorted array as command line arguments and performs binary search.
*/

#include <stdio.h>
#include <stdlib.h>

// Binary Search function
int binarySearch(int arr[], int n, int key) {
    int left = 0, right = n - 1;
    
    while(left <= right) {
        int mid = left + (right - left) / 2;
        
        if(arr[mid] == key)
            return mid;
        
        if(arr[mid] < key)
            left = mid + 1;
        else
            right = mid - 1;
    }
    
    return -1;
}

int main(int argc, char *argv[]) {
    if(argc < 3) {
        printf("Usage: %s <size> <element1> <element2> ... <elementN>\n", argv[0]);
        return 1;
    }
    
    int n = atoi(argv[1]);
    int arr[n];
    
    printf("\n=== Binary Search Program ===\n");
    printf("Received sorted array: ");
    
    for(int i = 0; i < n; i++) {
        arr[i] = atoi(argv[i + 2]);
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    int key;
    printf("\nEnter element to search: ");
    scanf("%d", &key);
    
    int result = binarySearch(arr, n, key);
    
    if(result != -1) {
        printf("Element %d found at index %d\n", key, result);
    } else {
        printf("Element %d not found in array\n", key);
    }
    
    return 0;
}
