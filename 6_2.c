// Assignment 6 - Problem 2: SCAN Disk Scheduling
#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, head, i, j, total = 0, size, direction;
    
    printf("Enter number of disk requests: ");
    scanf("%d", &n);
    
    int req[n+2];
    printf("Enter disk requests:\n");
    for(i = 0; i < n; i++) {
        scanf("%d", &req[i]);
    }
    
    printf("Enter initial head position: ");
    scanf("%d", &head);
    
    printf("Enter disk size: ");
    scanf("%d", &size);
    
    printf("Enter direction (0 for left, 1 for right): ");
    scanf("%d", &direction);
    
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
    
    // Find head position in sorted array
    int pos = 0;
    for(i = 0; i < n; i++) {
        if(head < req[i]) {
            pos = i;
            break;
        }
    }
    
    printf("\nSeek Sequence: %d", head);
    
    if(direction == 1) { // Moving right
        for(i = pos; i < n; i++) {
            total += abs(req[i] - head);
            head = req[i];
            printf(" -> %d", head);
        }
        total += abs((size-1) - head);
        head = size - 1;
        printf(" -> %d", head);
        
        for(i = pos-1; i >= 0; i--) {
            total += abs(req[i] - head);
            head = req[i];
            printf(" -> %d", head);
        }
    } else { // Moving left
        for(i = pos-1; i >= 0; i--) {
            total += abs(req[i] - head);
            head = req[i];
            printf(" -> %d", head);
        }
        total += abs(head - 0);
        head = 0;
        printf(" -> %d", head);
        
        for(i = pos; i < n; i++) {
            total += abs(req[i] - head);
            head = req[i];
            printf(" -> %d", head);
        }
    }
    
    printf("\n\nTotal Seek Time: %d\n", total);
    
    return 0;
}
