/*
==============================================================================
ASSIGNMENT 6.1: PAGE REPLACEMENT - FIFO & LRU
==============================================================================

PROBLEM:
- Generate random page reference string (0-9)
- Implement FIFO using queue concept
- Implement LRU using counter/timestamp method
- Variable frame size (1-7)
- Count and display page faults for each

COMPILATION:
g++ 6_1.cpp -o 6_1

EXECUTION:
./6_1

KEY FORMULAS:
1. Page Fault: When requested page not in frame
2. Page Hit: When requested page already in frame
3. Page Fault Rate = (Page Faults / Total References) × 100%

FIFO:
- Replace oldest page (first in, first out)
- Uses queue structure
- Simple but suffers from Belady's Anomaly

LRU (Least Recently Used):
- Replace page not used for longest time
- Track last use time for each page
- Better performance than FIFO
==============================================================================
*/

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

#define MAX_FRAMES 10
#define MAX_PAGES 50

// Check if page is in frames
int findPage(int frames[], int frameSize, int page) {
    for(int i = 0; i < frameSize; i++) {
        if(frames[i] == page)
            return i;  // Page found at index i
    }
    return -1;  // Page not found
}

// Display current frame state
void displayFrames(int frames[], int frameSize) {
    cout << "| ";
    for(int i = 0; i < frameSize; i++) {
        if(frames[i] == -1)
            cout << "- ";
        else
            cout << frames[i] << " ";
    }
    cout << "|";
}

// FIFO Page Replacement
int fifo(int pages[], int n, int frameSize) {
    int frames[MAX_FRAMES];
    int pageFaults = 0;
    int nextReplaceIndex = 0;  // Points to oldest page
    
    // Initialize frames as empty
    for(int i = 0; i < frameSize; i++)
        frames[i] = -1;
    
    cout << "\n--- FIFO Page Replacement ---" << endl;
    cout << "Reference  Frames           Fault/Hit" << endl;
    cout << "--------------------------------------" << endl;
    
    for(int i = 0; i < n; i++) {
        int page = pages[i];
        cout << "   " << page << "       ";
        
        // Check if page already in frame
        if(findPage(frames, frameSize, page) != -1) {
            // Page Hit
            displayFrames(frames, frameSize);
            cout << "  HIT" << endl;
        }
        else {
            // Page Fault
            pageFaults++;
            
            // Replace page at nextReplaceIndex (circular queue)
            frames[nextReplaceIndex] = page;
            nextReplaceIndex = (nextReplaceIndex + 1) % frameSize;
            
            displayFrames(frames, frameSize);
            cout << "  FAULT" << endl;
        }
    }
    
    return pageFaults;
}

// LRU Page Replacement using counter method
int lru(int pages[], int n, int frameSize) {
    int frames[MAX_FRAMES];
    int lastUsed[MAX_FRAMES];  // Track last use time
    int pageFaults = 0;
    
    // Initialize
    for(int i = 0; i < frameSize; i++) {
        frames[i] = -1;
        lastUsed[i] = -1;
    }
    
    cout << "\n--- LRU Page Replacement ---" << endl;
    cout << "Reference  Frames           Fault/Hit" << endl;
    cout << "--------------------------------------" << endl;
    
    for(int i = 0; i < n; i++) {
        int page = pages[i];
        cout << "   " << page << "       ";
        
        int pageIndex = findPage(frames, frameSize, page);
        
        if(pageIndex != -1) {
            // Page Hit - update last used time
            lastUsed[pageIndex] = i;
            displayFrames(frames, frameSize);
            cout << "  HIT" << endl;
        }
        else {
            // Page Fault
            pageFaults++;
            
            // Find victim (LRU page)
            int victimIndex = 0;
            int minTime = lastUsed[0];
            
            for(int j = 1; j < frameSize; j++) {
                if(lastUsed[j] < minTime) {
                    minTime = lastUsed[j];
                    victimIndex = j;
                }
            }
            
            // Replace LRU page
            frames[victimIndex] = page;
            lastUsed[victimIndex] = i;
            
            displayFrames(frames, frameSize);
            cout << "  FAULT" << endl;
        }
    }
    
    return pageFaults;
}

int main() {
    srand(time(0));
    
    cout << "========================================" << endl;
    cout << "  PAGE REPLACEMENT - FIFO & LRU" << endl;
    cout << "========================================\n" << endl;
    
    int n, frameSize;
    int pages[MAX_PAGES];
    
    // Input
    cout << "Enter number of page references: ";
    cin >> n;
    
    cout << "Enter frame size (1-7): ";
    cin >> frameSize;
    
    if(frameSize < 1 || frameSize > 7) {
        cout << "Invalid frame size! Must be 1-7" << endl;
        return 1;
    }
    
    cout << "\n1. Enter page reference string manually" << endl;
    cout << "2. Generate random page reference string (0-9)" << endl;
    cout << "Choice: ";
    int choice;
    cin >> choice;
    
    if(choice == 1) {
        cout << "\nEnter page reference string:" << endl;
        for(int i = 0; i < n; i++) {
            cin >> pages[i];
        }
    }
    else {
        // Generate random pages (0-9)
        cout << "\nGenerating random page reference string..." << endl;
        for(int i = 0; i < n; i++) {
            pages[i] = rand() % 10;
        }
    }
    
    // Display page reference string
    cout << "\nPage Reference String: ";
    for(int i = 0; i < n; i++) {
        cout << pages[i] << " ";
    }
    cout << endl;
    cout << "Frame Size: " << frameSize << endl;
    
    // FIFO
    cout << "\n========================================" << endl;
    int fifoFaults = fifo(pages, n, frameSize);
    cout << "\nTotal Page Faults (FIFO): " << fifoFaults << endl;
    cout << "Page Fault Rate (FIFO): " << (fifoFaults * 100.0 / n) << "%" << endl;
    
    // LRU
    cout << "\n========================================" << endl;
    int lruFaults = lru(pages, n, frameSize);
    cout << "\nTotal Page Faults (LRU): " << lruFaults << endl;
    cout << "Page Fault Rate (LRU): " << (lruFaults * 100.0 / n) << "%" << endl;
    
    // Comparison
    cout << "\n========================================" << endl;
    cout << "  COMPARISON" << endl;
    cout << "========================================" << endl;
    cout << "FIFO Page Faults: " << fifoFaults << endl;
    cout << "LRU Page Faults:  " << lruFaults << endl;
    
    if(fifoFaults < lruFaults)
        cout << "\nFIFO performed better!" << endl;
    else if(lruFaults < fifoFaults)
        cout << "\nLRU performed better!" << endl;
    else
        cout << "\nBoth performed equally!" << endl;
    
    cout << "\n========================================" << endl;
    
    return 0;
}

/*
==============================================================================
EXAM NOTES:

1. PAGE REPLACEMENT NEED:
   - Limited physical memory (frames)
   - Unlimited virtual memory (pages)
   - When frame full, must replace page
   - Goal: Minimize page faults

2. FIFO (First-In-First-Out):
   - Replace oldest page in memory
   - Simple to implement (queue)
   - Suffers from Belady's Anomaly
   - Not always optimal
   
   Algorithm:
   - Maintain queue of pages
   - On fault: Replace front, add to rear
   - O(1) replacement decision

3. LRU (Least Recently Used):
   - Replace page not used for longest time
   - Based on principle of locality
   - Better than FIFO usually
   - More complex to implement
   
   Algorithm:
   - Track last use time/counter
   - On fault: Find min time, replace
   - O(n) replacement decision

4. IMPLEMENTATION METHODS:

   FIFO:
   - Circular queue with pointer
   - nextReplace = (nextReplace + 1) % frames
   
   LRU:
   a) Counter method: Track time for each page
   b) Stack method: Most recent on top
   c) Aging: Shift bits based on usage

5. PAGE FAULT CALCULATION:
   Page Fault: Requested page not in any frame
   Page Hit: Requested page found in frame
   Fault Rate = (Faults / Total References) × 100

6. BELADY'S ANOMALY:
   - More frames → More page faults (FIFO)
   - Counterintuitive behavior
   - Example: 1,2,3,4,1,2,5,1,2,3,4,5
     3 frames: 9 faults, 4 frames: 10 faults!
   - Doesn't occur in LRU or Optimal

7. EXAMPLE EXECUTION:
   Reference: 7 0 1 2 0 3 0
   Frames: 3
   
   FIFO:
   7: [7 - -] FAULT
   0: [7 0 -] FAULT
   1: [7 0 1] FAULT
   2: [2 0 1] FAULT (replace 7)
   0: [2 0 1] HIT
   3: [2 3 1] FAULT (replace 0)
   0: [2 3 0] FAULT (replace 1)
   Total: 6 faults
   
   LRU:
   7: [7 - -] FAULT
   0: [7 0 -] FAULT
   1: [7 0 1] FAULT
   2: [2 0 1] FAULT (replace 7, LRU)
   0: [2 0 1] HIT
   3: [2 0 3] FAULT (replace 1, LRU)
   0: [2 0 3] HIT
   Total: 5 faults

8. TIME COMPLEXITY:
   FIFO: O(n) for n references, O(1) per reference
   LRU: O(n × f) where f = frame size
        Can optimize to O(n log f) with heap

9. SPACE COMPLEXITY:
   FIFO: O(f) for frames
   LRU: O(f) for frames + O(f) for counters

10. EXAM TIP:
    - Draw frame state at each step
    - Mark FAULT/HIT clearly
    - For LRU, show time/counter values
    - Calculate fault rate at end
    - Explain replacement decision

11. WHICH IS BETTER?
    LRU usually better (locality principle)
    But FIFO simpler to implement
    Optimal is theoretical best (needs future)
==============================================================================
*/
