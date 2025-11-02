// Assignment 3 - Problem 2: LRU Page Replacement
#include <stdio.h>

int main() {
    int n, frames, i, j, k, flag, count = 0, pos, min, found;
    
    printf("Enter number of pages: ");
    scanf("%d", &n);
    
    int pages[n];
    printf("Enter page reference string:\n");
    for(i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }
    
    printf("Enter number of frames: ");
    scanf("%d", &frames);
    
    int frame[frames], recent[frames];
    for(i = 0; i < frames; i++) {
        frame[i] = -1;
        recent[i] = 0;
    }
    
    printf("\nPage\tFrames\n");
    
    for(i = 0; i < n; i++) {
        flag = 0;
        
        // Check if page already in frame
        for(j = 0; j < frames; j++) {
            if(frame[j] == pages[i]) {
                flag = 1;
                recent[j] = i;
                break;
            }
        }
        
        if(flag == 0) {
            // Find LRU page
            min = 0;
            for(j = 1; j < frames; j++) {
                if(recent[j] < recent[min]) {
                    min = j;
                }
            }
            
            frame[min] = pages[i];
            recent[min] = i;
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
