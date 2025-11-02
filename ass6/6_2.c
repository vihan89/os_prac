/*
6.2 Consider the page reference string of size 12: 1, 2, 3, 4, 1, 2, 5, 1, 1, 2, 3, 4, 5 
with frame size 3 and 4 respectively. 
Write a C program to simulate page replacement algorithms a) Optimal b) LRU.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PAGES 20
#define MAX_FRAMES 10

// Function to check if page is in frame
int findPage(int frames[], int num_frames, int page) {
    for(int i = 0; i < num_frames; i++) {
        if(frames[i] == page) {
            return i;
        }
    }
    return -1;
}

// Optimal Page Replacement Algorithm
int optimal(int pages[], int n, int num_frames) {
    int frames[MAX_FRAMES];
    int page_faults = 0;
    
    // Initialize frames to -1
    for(int i = 0; i < num_frames; i++) {
        frames[i] = -1;
    }
    
    printf("\n===== Optimal Page Replacement Algorithm =====\n");
    printf("Page\tFrames\t\t\tPage Fault\n");
    printf("----------------------------------------------------\n");
    
    for(int i = 0; i < n; i++) {
        int page = pages[i];
        
        // Check if page is already in frame
        if(findPage(frames, num_frames, page) == -1) {
            // Page fault occurs
            int replace_index = -1;
            
            // Find empty frame
            for(int j = 0; j < num_frames; j++) {
                if(frames[j] == -1) {
                    replace_index = j;
                    break;
                }
            }
            
            // If no empty frame, find optimal page to replace
            if(replace_index == -1) {
                int farthest = i;
                
                for(int j = 0; j < num_frames; j++) {
                    int next_use = -1;
                    
                    // Find when this frame's page will be used next
                    for(int k = i + 1; k < n; k++) {
                        if(frames[j] == pages[k]) {
                            next_use = k;
                            break;
                        }
                    }
                    
                    // If page won't be used again, replace it
                    if(next_use == -1) {
                        replace_index = j;
                        break;
                    }
                    
                    // Find the page that will be used farthest in future
                    if(next_use > farthest) {
                        farthest = next_use;
                        replace_index = j;
                    }
                }
            }
            
            frames[replace_index] = page;
            page_faults++;
            
            printf("%d\t", page);
            for(int j = 0; j < num_frames; j++) {
                if(frames[j] != -1)
                    printf("%d ", frames[j]);
                else
                    printf("- ");
            }
            printf("\t\tYes\n");
        } else {
            printf("%d\t", page);
            for(int j = 0; j < num_frames; j++) {
                if(frames[j] != -1)
                    printf("%d ", frames[j]);
                else
                    printf("- ");
            }
            printf("\t\tNo\n");
        }
    }
    
    return page_faults;
}

// LRU Page Replacement Algorithm
int lru(int pages[], int n, int num_frames) {
    int frames[MAX_FRAMES];
    int recent[MAX_FRAMES];  // Tracks recent usage time
    int page_faults = 0;
    
    // Initialize frames and recent
    for(int i = 0; i < num_frames; i++) {
        frames[i] = -1;
        recent[i] = -1;
    }
    
    printf("\n===== LRU Page Replacement Algorithm =====\n");
    printf("Page\tFrames\t\t\tPage Fault\n");
    printf("----------------------------------------------------\n");
    
    for(int i = 0; i < n; i++) {
        int page = pages[i];
        int pos = findPage(frames, num_frames, page);
        
        if(pos == -1) {
            // Page fault occurs
            // Find LRU page
            int lru_pos = 0;
            for(int j = 1; j < num_frames; j++) {
                if(recent[j] < recent[lru_pos]) {
                    lru_pos = j;
                }
            }
            
            frames[lru_pos] = page;
            recent[lru_pos] = i;
            page_faults++;
            
            printf("%d\t", page);
            for(int j = 0; j < num_frames; j++) {
                if(frames[j] != -1)
                    printf("%d ", frames[j]);
                else
                    printf("- ");
            }
            printf("\t\tYes\n");
        } else {
            // Update recent usage
            recent[pos] = i;
            
            printf("%d\t", page);
            for(int j = 0; j < num_frames; j++) {
                if(frames[j] != -1)
                    printf("%d ", frames[j]);
                else
                    printf("- ");
            }
            printf("\t\tNo\n");
        }
    }
    
    return page_faults;
}

void displayPageString(int pages[], int n) {
    printf("Page Reference String: ");
    for(int i = 0; i < n; i++) {
        printf("%d", pages[i]);
        if(i < n - 1) printf(", ");
    }
    printf("\n");
}

int main() {
    // Given page reference string
    int pages[] = {1, 2, 3, 4, 1, 2, 5, 1, 1, 2, 3, 4, 5};
    int n = 13;
    int num_frames;
    int choice;
    
    printf("========================================\n");
    printf("  PAGE REPLACEMENT ALGORITHMS\n");
    printf("  (Optimal and LRU)\n");
    printf("========================================\n\n");
    
    displayPageString(pages, n);
    printf("Page Reference String Size: %d\n", n);
    
    while(1) {
        printf("\n========================================\n");
        printf("              MENU\n");
        printf("========================================\n");
        printf("1. Optimal with 3 frames\n");
        printf("2. Optimal with 4 frames\n");
        printf("3. LRU with 3 frames\n");
        printf("4. LRU with 4 frames\n");
        printf("5. Compare Optimal vs LRU (3 frames)\n");
        printf("6. Compare Optimal vs LRU (4 frames)\n");
        printf("7. Compare 3 frames vs 4 frames\n");
        printf("8. Exit\n");
        printf("========================================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: {
                num_frames = 3;
                printf("\nFrame Size: %d\n", num_frames);
                int faults = optimal(pages, n, num_frames);
                printf("\nTotal Page Faults: %d\n", faults);
                printf("Hit Ratio: %.2f%%\n", ((n - faults) * 100.0) / n);
                printf("Miss Ratio: %.2f%%\n", (faults * 100.0) / n);
                break;
            }
            
            case 2: {
                num_frames = 4;
                printf("\nFrame Size: %d\n", num_frames);
                int faults = optimal(pages, n, num_frames);
                printf("\nTotal Page Faults: %d\n", faults);
                printf("Hit Ratio: %.2f%%\n", ((n - faults) * 100.0) / n);
                printf("Miss Ratio: %.2f%%\n", (faults * 100.0) / n);
                break;
            }
            
            case 3: {
                num_frames = 3;
                printf("\nFrame Size: %d\n", num_frames);
                int faults = lru(pages, n, num_frames);
                printf("\nTotal Page Faults: %d\n", faults);
                printf("Hit Ratio: %.2f%%\n", ((n - faults) * 100.0) / n);
                printf("Miss Ratio: %.2f%%\n", (faults * 100.0) / n);
                break;
            }
            
            case 4: {
                num_frames = 4;
                printf("\nFrame Size: %d\n", num_frames);
                int faults = lru(pages, n, num_frames);
                printf("\nTotal Page Faults: %d\n", faults);
                printf("Hit Ratio: %.2f%%\n", ((n - faults) * 100.0) / n);
                printf("Miss Ratio: %.2f%%\n", (faults * 100.0) / n);
                break;
            }
            
            case 5: {
                num_frames = 3;
                printf("\nComparison with %d frames:\n", num_frames);
                printf("========================================\n");
                
                int opt_faults = optimal(pages, n, num_frames);
                int lru_faults = lru(pages, n, num_frames);
                
                printf("\n========================================\n");
                printf("         COMPARISON RESULTS\n");
                printf("========================================\n");
                printf("Algorithm\tPage Faults\tHit Ratio\n");
                printf("----------------------------------------------------\n");
                printf("Optimal\t\t%d\t\t%.2f%%\n", opt_faults, ((n - opt_faults) * 100.0) / n);
                printf("LRU\t\t%d\t\t%.2f%%\n", lru_faults, ((n - lru_faults) * 100.0) / n);
                printf("----------------------------------------------------\n");
                
                if(opt_faults < lru_faults) {
                    printf("Optimal performs better by %d page faults!\n", lru_faults - opt_faults);
                } else if(lru_faults < opt_faults) {
                    printf("LRU performs better by %d page faults!\n", opt_faults - lru_faults);
                } else {
                    printf("Both algorithms perform equally!\n");
                }
                break;
            }
            
            case 6: {
                num_frames = 4;
                printf("\nComparison with %d frames:\n", num_frames);
                printf("========================================\n");
                
                int opt_faults = optimal(pages, n, num_frames);
                int lru_faults = lru(pages, n, num_frames);
                
                printf("\n========================================\n");
                printf("         COMPARISON RESULTS\n");
                printf("========================================\n");
                printf("Algorithm\tPage Faults\tHit Ratio\n");
                printf("----------------------------------------------------\n");
                printf("Optimal\t\t%d\t\t%.2f%%\n", opt_faults, ((n - opt_faults) * 100.0) / n);
                printf("LRU\t\t%d\t\t%.2f%%\n", lru_faults, ((n - lru_faults) * 100.0) / n);
                printf("----------------------------------------------------\n");
                
                if(opt_faults < lru_faults) {
                    printf("Optimal performs better by %d page faults!\n", lru_faults - opt_faults);
                } else if(lru_faults < opt_faults) {
                    printf("LRU performs better by %d page faults!\n", opt_faults - lru_faults);
                } else {
                    printf("Both algorithms perform equally!\n");
                }
                break;
            }
            
            case 7: {
                printf("\n========================================\n");
                printf("   FRAME SIZE COMPARISON\n");
                printf("========================================\n");
                
                int opt_3 = optimal(pages, n, 3);
                int opt_4 = optimal(pages, n, 4);
                int lru_3 = lru(pages, n, 3);
                int lru_4 = lru(pages, n, 4);
                
                printf("\n========================================\n");
                printf("         COMPARISON RESULTS\n");
                printf("========================================\n");
                printf("Algorithm\t3 Frames\t4 Frames\tDifference\n");
                printf("----------------------------------------------------\n");
                printf("Optimal\t\t%d\t\t%d\t\t%d\n", opt_3, opt_4, opt_3 - opt_4);
                printf("LRU\t\t%d\t\t%d\t\t%d\n", lru_3, lru_4, lru_3 - lru_4);
                printf("----------------------------------------------------\n");
                printf("\nNote: Increasing frames generally reduces page faults.\n");
                break;
            }
            
            case 8:
                printf("\nExiting...\n");
                exit(0);
            
            default:
                printf("\nInvalid choice!\n");
        }
    }
    
    return 0;
}
