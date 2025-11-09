#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int page_ref[50], n;

void optimal(int frames) {
    int mem[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int faults = 0;
    
    printf("\n=== OPTIMAL ===\n");
    printf("Ref  Frames\n");
    
    for(int i = 0; i < n; i++) {
        int pos = -1;
        for(int j = 0; j < frames; j++) {
            if(mem[j] == page_ref[i]) {
                pos = j;
                break;
            }
        }
        
        printf("%d    ", page_ref[i]);
        if(pos == -1) {
            faults++;
            int victim = 0;
            
            int empty = -1;
            for(int j = 0; j < frames; j++) {
                if(mem[j] == -1) {
                    empty = j;
                    break;
                }
            }
            
            if(empty != -1) {
                mem[empty] = page_ref[i];
            } else {
                int farthest = -1;
                for(int j = 0; j < frames; j++) {
                    int next_use = 9999;
                    for(int k = i + 1; k < n; k++) {
                        if(page_ref[k] == mem[j]) {
                            next_use = k;
                            break;
                        }
                    }
                    if(next_use > farthest) {
                        farthest = next_use;
                        victim = j;
                    }
                }
                mem[victim] = page_ref[i];
            }
        }
        
        for(int j = 0; j < frames; j++) {
            if(mem[j] == -1) printf("- ");
            else printf("%d ", mem[j]);
        }
        printf(pos == -1 ? " FAULT\n" : " HIT\n");
    }
    printf("Total Faults: %d\n", faults);
}

void lru(int frames) {
    int mem[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int time[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int faults = 0;
    
    printf("\n=== LRU ===\n");
    printf("Ref  Frames\n");
    
    for(int i = 0; i < n; i++) {
        int pos = -1;
        for(int j = 0; j < frames; j++) {
            if(mem[j] == page_ref[i]) {
                pos = j;
                break;
            }
        }
        
        printf("%d    ", page_ref[i]);
        if(pos != -1) {
            time[pos] = i;
        } else {
            faults++;
            int lru_idx = 0, min_time = time[0];
            for(int j = 1; j < frames; j++) {
                if(time[j] < min_time) {
                    min_time = time[j];
                    lru_idx = j;
                }
            }
            mem[lru_idx] = page_ref[i];
            time[lru_idx] = i;
        }
        
        for(int j = 0; j < frames; j++) {
            if(mem[j] == -1) printf("- ");
            else printf("%d ", mem[j]);
        }
        printf(pos != -1 ? " HIT\n" : " FAULT\n");
    }
    printf("Total Faults: %d\n", faults);
}

int main() {
    int frames;
    
    printf("Enter number of references: ");
    scanf("%d", &n);
    printf("Enter number of frames: ");
    scanf("%d", &frames);
    
    printf("Enter page reference string:\n");
    for(int i = 0; i < n; i++) scanf("%d", &page_ref[i]);
    
    printf("\n1. OPTIMAL\n2. LRU\nChoice: ");
    int choice;
    scanf("%d", &choice);
    
    if(choice == 1) optimal(frames);
    else if(choice == 2) lru(frames);
    
    return 0;
}
