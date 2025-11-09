// OPTIMAL and LRU Page Replacement - Simple version
#include <iostream>
using namespace std;

// OPTIMAL: Replace page not used for longest time in future
void optimal(int ref[], int n, int frames) {
    int mem[10], faults = 0;
    for(int i = 0; i < 10; i++) mem[i] = -1;
    
    cout << "\n=== OPTIMAL ===\n";
    for(int i = 0; i < n; i++) {
        // Check if page already in memory
        int pos = -1;
        for(int j = 0; j < frames; j++) {
            if(mem[j] == ref[i]) {
                pos = j;
                break;
            }
        }
        
        if(pos == -1) {
            // Find empty slot first
            int victim = -1;
            for(int j = 0; j < frames; j++) {
                if(mem[j] == -1) {
                    victim = j;
                    break;
                }
            }
            
            // If no empty slot, find page used farthest in future
            if(victim == -1) {
                int farthest = -1;
                for(int j = 0; j < frames; j++) {
                    int next = 9999;
                    for(int k = i + 1; k < n; k++) {
                        if(ref[k] == mem[j]) {
                            next = k;
                            break;
                        }
                    }
                    if(next > farthest) {
                        farthest = next;
                        victim = j;
                    }
                }
            }
            
            mem[victim] = ref[i];
            faults++;
        }
        
        // Display memory state
        cout << ref[i] << " -> ";
        for(int j = 0; j < frames; j++)
            cout << (mem[j] == -1 ? "-" : to_string(mem[j])) << " ";
        cout << (pos == -1 ? " FAULT\n" : " HIT\n");
    }
    cout << "Total Faults: " << faults << "\n";
}

// LRU: Replace least recently used page
void lru(int ref[], int n, int frames) {
    int mem[10], time[10], faults = 0;
    for(int i = 0; i < 10; i++) mem[i] = time[i] = -1;
    
    cout << "\n=== LRU ===\n";
    for(int i = 0; i < n; i++) {
        // Check if page already in memory
        int pos = -1;
        for(int j = 0; j < frames; j++) {
            if(mem[j] == ref[i]) {
                pos = j;
                break;
            }
        }
        
        if(pos != -1) {
            // Hit: update time
            time[pos] = i;
        } else {
            // Fault: find LRU and replace
            int lru = 0;
            for(int j = 1; j < frames; j++)
                if(time[j] < time[lru]) lru = j;
            
            mem[lru] = ref[i];
            time[lru] = i;
            faults++;
        }
        
        // Display memory state
        cout << ref[i] << " -> ";
        for(int j = 0; j < frames; j++)
            cout << (mem[j] == -1 ? "-" : to_string(mem[j])) << " ";
        cout << (pos != -1 ? " HIT\n" : " FAULT\n");
    }
    cout << "Total Faults: " << faults << "\n";
}

int main() {
    int n, ref[50];
    
    cout << "Number of page references: "; cin >> n;
    cout << "Enter page reference string: ";
    for(int i = 0; i < n; i++) cin >> ref[i];
    
    cout << "\n========== FRAME SIZE = 3 ==========";
    optimal(ref, n, 3);
    lru(ref, n, 3);
    
    cout << "\n========== FRAME SIZE = 4 ==========";
    optimal(ref, n, 4);
    lru(ref, n, 4);
    
    return 0;
}