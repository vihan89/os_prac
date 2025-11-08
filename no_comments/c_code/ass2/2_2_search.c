#include <stdio.h>
#include <stdlib.h>

int binarySearch(int arr[], int n, int key) {
    int l = 0, r = n - 1;
    while(l <= r) {
        int m = l + (r - l) / 2;
        if(arr[m] == key) return m;
        if(arr[m] < key) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

int main(int argc, char* argv[]) {
    if(argc < 4) {
        printf("Usage: %s <n> <arr...> <key>\n", argv[0]);
        return 1;
    }
    
    int n = atoi(argv[1]);
    int arr[100];
    
    for(int i = 0; i < n; i++) arr[i] = atoi(argv[i + 2]);
    int key = atoi(argv[n + 2]);
    
    int result = binarySearch(arr, n, key);
    
    if(result != -1)
        printf("Found %d at index %d\n", key, result);
    else
        printf("%d not found\n", key);
    
    return (result != -1) ? 0 : 1;
}
