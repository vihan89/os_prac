#include <stdio.h>
#include <stdbool.h>

void fifo(int ref[], int n, int frames) {
    int mem[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int idx = 0, faults = 0;
    
    printf("\n=== FIFO ===\n");
    printf("Ref  Frames\n");
    
    for(int i = 0; i < n; i++) {
        bool found = false;
        for(int j = 0; j < frames; j++) {
            if(mem[j] == ref[i]) {
                found = true;
                break;
            }
        }
        
        printf("%d    ", ref[i]);
        if(!found) {
            mem[idx] = ref[i];
            idx = (idx + 1) % frames;
            faults++;
        }
        
        for(int j = 0; j < frames; j++) {
            if(mem[j] == -1) printf("- ");
            else printf("%d ", mem[j]);
        }
        printf(found ? " HIT\n" : " FAULT\n");
    }
    printf("Total Faults: %d\n", faults);
}

void lru(int ref[], int n, int frames) {
    int mem[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int time[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int faults = 0;
    
    printf("\n=== LRU ===\n");
    printf("Ref  Frames\n");
    
    for(int i = 0; i < n; i++) {
        int pos = -1;
        for(int j = 0; j < frames; j++) {
            if(mem[j] == ref[i]) {
                pos = j;
                break;
            }
        }
        
        printf("%d    ", ref[i]);
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
            mem[lru_idx] = ref[i];
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
    int n, frames, ref[50];
    
    printf("Enter number of references: ");
    scanf("%d", &n);
    printf("Enter number of frames: ");
    scanf("%d", &frames);
    
    printf("Enter page reference string:\n");
    for(int i = 0; i < n; i++) scanf("%d", &ref[i]);
    
    printf("\n1. FIFO\n2. LRU\nChoice: ");
    int choice;
    scanf("%d", &choice);
    
    if(choice == 1) fifo(ref, n, frames);
    else if(choice == 2) lru(ref, n, frames);
    
    return 0;
}
