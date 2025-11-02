// Assignment 5 - Problem 3: Worst Fit Memory Allocation
#include <stdio.h>

int main() {
    int nb, np, i, j;
    
    printf("Enter number of blocks: ");
    scanf("%d", &nb);
    
    int blocks[nb], original[nb];
    printf("Enter size of each block:\n");
    for(i = 0; i < nb; i++) {
        scanf("%d", &blocks[i]);
        original[i] = blocks[i];
    }
    
    printf("Enter number of processes: ");
    scanf("%d", &np);
    
    int process[np], allocation[np];
    printf("Enter size of each process:\n");
    for(i = 0; i < np; i++) {
        scanf("%d", &process[i]);
        allocation[i] = -1;
    }
    
    // Worst Fit allocation
    for(i = 0; i < np; i++) {
        int worst = -1;
        for(j = 0; j < nb; j++) {
            if(blocks[j] >= process[i]) {
                if(worst == -1 || blocks[j] > blocks[worst]) {
                    worst = j;
                }
            }
        }
        
        if(worst != -1) {
            allocation[i] = worst;
            blocks[worst] -= process[i];
        }
    }
    
    // Display results
    printf("\nProcess\tSize\tBlock\tBlock Size\n");
    for(i = 0; i < np; i++) {
        printf("P%d\t%d\t", i+1, process[i]);
        if(allocation[i] != -1) {
            printf("%d\t%d\n", allocation[i]+1, original[allocation[i]]);
        } else {
            printf("Not Allocated\t-\n");
        }
    }
    
    return 0;
}
