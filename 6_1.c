// Assignment 6 - Problem 1: FCFS Disk Scheduling
#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, head, i, total = 0;
    
    printf("Enter number of disk requests: ");
    scanf("%d", &n);
    
    int req[n];
    printf("Enter disk requests:\n");
    for(i = 0; i < n; i++) {
        scanf("%d", &req[i]);
    }
    
    printf("Enter initial head position: ");
    scanf("%d", &head);
    
    printf("\nSeek Sequence: %d", head);
    
    for(i = 0; i < n; i++) {
        total += abs(req[i] - head);
        head = req[i];
        printf(" -> %d", head);
    }
    
    printf("\n\nTotal Seek Time: %d\n", total);
    printf("Average Seek Time: %.2f\n", (float)total/n);
    
    return 0;
}
