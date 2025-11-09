// FIFO and LRU Page Replacement - Random page generation
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// FIFO: Replace oldest page (circular queue)
int fifo(int ref[], int n, int frames) {
    int mem[10], idx = 0, faults = 0;
    for(int i = 0; i < 10; i++) mem[i] = -1;
    
    for(int i = 0; i < n; i++) {
        // Check if page already in memory
        bool hit = false;
        for(int j = 0; j < frames; j++) {
            if(mem[j] == ref[i]) {
                hit = true;
                break;
            }
        }
        
        // If not found, replace oldest
        if(!hit) {
            mem[idx] = ref[i];
            idx = (idx + 1) % frames;  // circular
            faults++;
        }
    }
    return faults;
}

// LRU: Replace least recently used page
int lru(int ref[], int n, int frames) {
    int mem[10], time[10], faults = 0;
    for(int i = 0; i < 10; i++) mem[i] = time[i] = -1;
    
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
    }
    return faults;
}

int main() {
    int n, ref[50];
    
    cout << "References: "; cin >> n;
    
    // Generate random pages (0-9)
    srand(time(0));
    cout << "Pages: ";
    for(int i = 0; i < n; i++) {
        ref[i] = rand() % 10;
        cout << ref[i] << " ";
    }
    
    cout << "\n\nFrames  FIFO  LRU\n";
    for(int f = 1; f <= 7; f++)
        cout << "  " << f << "      " << fifo(ref, n, f) << "     " << lru(ref, n, f) << "\n";
    
    return 0;
}

