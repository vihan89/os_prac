// Assignment 6 - Problem 3: C-SCAN Disk Scheduling
#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, head, i, j, total = 0, size;
    
    printf("Enter number of disk requests: ");
    scanf("%d", &n);
    
    int req[n];
    printf("Enter disk requests:\n");
    for(i = 0; i < n; i++) {
        scanf("%d", &req[i]);
    }
    
    printf("Enter initial head position: ");
    scanf("%d", &head);
    
    printf("Enter disk size: ");
    scanf("%d", &size);
    
    // Sort requests
    for(i = 0; i < n-1; i++) {
        for(j = i+1; j < n; j++) {
            if(req[i] > req[j]) {
                int temp = req[i];
                req[i] = req[j];
                req[j] = temp;
            }
        }
    }
    
    // Find head position
    int pos = 0;
    for(i = 0; i < n; i++) {
        if(head < req[i]) {
            pos = i;
            break;
        }
    }
    
    printf("\nSeek Sequence: %d", head);
    
    // Move right to end
    for(i = pos; i < n; i++) {
        total += abs(req[i] - head);
        head = req[i];
        printf(" -> %d", head);
    }
    
    // Jump to start
    total += abs((size-1) - head);
    total += (size-1);
    head = 0;
    printf(" -> %d -> %d", size-1, head);
    
    // Service remaining requests
    for(i = 0; i < pos; i++) {
        total += abs(req[i] - head);
        head = req[i];
        printf(" -> %d", head);
    }
    
    printf("\n\nTotal Seek Time: %d\n", total);
    
    return 0;
}
