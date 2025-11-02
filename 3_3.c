// Assignment 3 - Problem 3: Optimal Page Replacement
#include <stdio.h>

int main() {
    int n, frames, i, j, k, flag, count = 0, pos, max, index;
    
    printf("Enter number of pages: ");
    scanf("%d", &n);
    
    int pages[n];
    printf("Enter page reference string:\n");
    for(i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }
    
    printf("Enter number of frames: ");
    scanf("%d", &frames);
    
    int frame[frames], future[frames];
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
            // Find optimal page to replace
            for(j = 0; j < frames; j++) {
                future[j] = n;
                for(k = i + 1; k < n; k++) {
                    if(frame[j] == pages[k]) {
                        future[j] = k;
                        break;
                    }
                }
            }
            
            max = 0;
            for(j = 1; j < frames; j++) {
                if(future[j] > future[max]) {
                    max = j;
                }
            }
            
            frame[max] = pages[i];
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
