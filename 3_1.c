// Assignment 3 - Problem 1: FIFO Page Replacement
#include <stdio.h>

int main() {
    int n, frames, i, j, k, flag, count = 0, pos = 0;
    
    printf("Enter number of pages: ");
    scanf("%d", &n);
    
    int pages[n];
    printf("Enter page reference string:\n");
    for(i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }
    
    printf("Enter number of frames: ");
    scanf("%d", &frames);
    
    int frame[frames];
    for(i = 0; i < frames; i++) {
        frame[i] = -1;
    }
    
    printf("\nPage\tFrames\n");
    
    for(i = 0; i < n; i++) {
        flag = 0;
        
        // Check if page already in frame
        for(j = 0; j < frames; j++) {
            if(frame[j] == pages[i]) {
                flag = 1;
                break;
            }
        }
        
        if(flag == 0) {
            frame[pos] = pages[i];
            pos = (pos + 1) % frames;
            count++;
        }
        
        printf("%d\t", pages[i]);
        for(k = 0; k < frames; k++) {
            if(frame[k] != -1)
                printf("%d ", frame[k]);
            else
                printf("- ");
        }
        printf("\n");
    }
    
    printf("\nTotal Page Faults: %d\n", count);
    
    return 0;
}
