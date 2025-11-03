/*
6.1 Write a C program to simulate page replacement algorithms a) FIFO b) LRU. 
First, generate a random page-reference string where page numbers range from 0 to 9. 
Apply the random page-reference string to each algorithm and record the number of 
page faults incurred by each algorithm. 
Implement the replacement algorithms so that the number of page frames can vary from 1 to 7.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_PAGES 30
#define MAX_FRAMES 7
#define PAGE_RANGE 10

// Function to check if page is in frame
int findPage(int frames[], int num_frames, int page) {
    for(int i = 0; i < num_frames; i++) {
        if(frames[i] == page) {
            return i;
        }
    }
    return -1;
}

// FIFO Page Replacement Algorithm
int fifo(int pages[], int n, int num_frames) {
    int frames[MAX_FRAMES];
    int page_faults = 0;
    int front = 0;
    
    // Initialize frames to -1
    for(int i = 0; i < num_frames; i++) {
        frames[i] = -1;
    }
    
    printf("\n===== FIFO Algorithm =====\n");
    printf("Page\tFrames\t\t\tPage Fault\n");
    printf("----------------------------------------------------\n");
    
    for(int i = 0; i < n; i++) {
        int page = pages[i];
        
        // Check if page is already in frame
        if(findPage(frames, num_frames, page) == -1) {
            // Page fault occurs
            frames[front] = page;
            front = (front + 1) % num_frames;
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
    int recent[MAX_FRAMES];  // Tracks recent usage
    int page_faults = 0;
    
    // Initialize frames and recent
    for(int i = 0; i < num_frames; i++) {
        frames[i] = -1;
        recent[i] = -1;
    }
    
    printf("\n===== LRU Algorithm =====\n");
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

// Function to generate random page reference string
void generatePageReference(int pages[], int n) {
    for(int i = 0; i < n; i++) {
        pages[i] = rand() % PAGE_RANGE;
    }
}

int main() {
    int pages[MAX_PAGES];
    int n, num_frames;
    int choice;
    
    srand(time(NULL));
    
    printf("========================================\n");
    printf("  PAGE REPLACEMENT ALGORITHMS\n");
    printf("========================================\n\n");
    
    printf("Enter length of page reference string (max %d): ", MAX_PAGES);
    scanf("%d", &n);
    
    if(n > MAX_PAGES) {
        printf("Error: Maximum allowed is %d\n", MAX_PAGES);
        return 1;
    }
    
    // Generate random page reference string
    generatePageReference(pages, n);
    
    printf("\nGenerated Page Reference String:\n");
    for(int i = 0; i < n; i++) {
        printf("%d ", pages[i]);
    }
    printf("\n");
    
    printf("\nEnter number of frames (1-7): ");
    scanf("%d", &num_frames);
    
    if(num_frames < 1 || num_frames > 7) {
        printf("Error: Number of frames must be between 1 and 7\n");
        return 1;
    }
    
    while(1) {
        printf("\n========================================\n");
        printf("              MENU\n");
        printf("========================================\n");
        printf("1. FIFO Page Replacement\n");
        printf("2. LRU Page Replacement\n");
        printf("3. Compare Both Algorithms\n");
        printf("4. Generate New Page Reference String\n");
        printf("5. Change Number of Frames\n");
        printf("6. Exit\n");
        printf("========================================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: {
                int faults = fifo(pages, n, num_frames);
                printf("\nTotal Page Faults (FIFO): %d\n", faults);
                printf("Page Fault Rate: %.2f%%\n", (faults * 100.0) / n);
                break;
            }
            
            case 2: {
                int faults = lru(pages, n, num_frames);
                printf("\nTotal Page Faults (LRU): %d\n", faults);
                printf("Page Fault Rate: %.2f%%\n", (faults * 100.0) / n);
                break;
            }
            
            case 3: {
                int fifo_faults = fifo(pages, n, num_frames);
                int lru_faults = lru(pages, n, num_frames);
                
                printf("\n========================================\n");
                printf("         COMPARISON RESULTS\n");
                printf("========================================\n");
                printf("Algorithm\tPage Faults\tFault Rate\n");
                printf("----------------------------------------------------\n");
                printf("FIFO\t\t%d\t\t%.2f%%\n", fifo_faults, (fifo_faults * 100.0) / n);
                printf("LRU\t\t%d\t\t%.2f%%\n", lru_faults, (lru_faults * 100.0) / n);
                printf("----------------------------------------------------\n");
                
                if(fifo_faults < lru_faults) {
                    printf("FIFO performs better!\n");
                } else if(lru_faults < fifo_faults) {
                    printf("LRU performs better!\n");
                } else {
                    printf("Both algorithms perform equally!\n");
                }
                break;
            }
            
            case 4:
                generatePageReference(pages, n);
                printf("\nNew Page Reference String:\n");
                for(int i = 0; i < n; i++) {
                    printf("%d ", pages[i]);
                }
                printf("\n");
                break;
            
            case 5:
                printf("\nEnter number of frames (1-7): ");
                scanf("%d", &num_frames);
                if(num_frames < 1 || num_frames > 7) {
                    printf("Error: Number of frames must be between 1 and 7\n");
                    num_frames = 3;
                }
                printf("Number of frames set to: %d\n", num_frames);
                break;
            
            case 6:
                printf("\nExiting...\n");
                exit(0);
            
            default:
                printf("\nInvalid choice!\n");
        }
    }
    
    return 0;
}
