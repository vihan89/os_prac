// Assignment 4 - Problem 1: Banker's Algorithm
#include <stdio.h>

int main() {
    int n, m, i, j, k;
    
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    printf("Enter number of resources: ");
    scanf("%d", &m);
    
    int alloc[n][m], max[n][m], avail[m], need[n][m];
    int finish[n], safeSeq[n], work[m];
    
    printf("\nEnter Allocation Matrix:\n");
    for(i = 0; i < n; i++) {
        for(j = 0; j < m; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }
    
    printf("\nEnter Max Matrix:\n");
    for(i = 0; i < n; i++) {
        for(j = 0; j < m; j++) {
            scanf("%d", &max[i][j]);
        }
    }
    
    printf("\nEnter Available Resources:\n");
    for(i = 0; i < m; i++) {
        scanf("%d", &avail[i]);
    }
    
    // Calculate Need Matrix
    for(i = 0; i < n; i++) {
        for(j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
    
    // Initialize
    for(i = 0; i < n; i++) {
        finish[i] = 0;
    }
    
    for(i = 0; i < m; i++) {
        work[i] = avail[i];
    }
    
    // Find safe sequence
    int count = 0;
    while(count < n) {
        int found = 0;
        for(i = 0; i < n; i++) {
            if(finish[i] == 0) {
                int flag = 1;
                for(j = 0; j < m; j++) {
                    if(need[i][j] > work[j]) {
                        flag = 0;
                        break;
                    }
                }
                
                if(flag == 1) {
                    for(k = 0; k < m; k++) {
                        work[k] += alloc[i][k];
                    }
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        
        if(found == 0) {
            printf("\nSystem is not in safe state!\n");
            return 0;
        }
    }
    
    printf("\nSystem is in safe state.\nSafe Sequence: ");
    for(i = 0; i < n; i++) {
        printf("P%d ", safeSeq[i]);
    }
    printf("\n");
    
    return 0;
}
