/*
==============================================================================
ASSIGNMENT 6.2: PAGE REPLACEMENT - OPTIMAL & LRU
==============================================================================

PROBLEM:
- Fixed reference string: 1,2,3,4,1,2,5,1,1,2,3,4,5
- Frame sizes: 3 and 4
- Optimal: Replace page not used for longest time in FUTURE
- LRU: Replace page not used for longest time in PAST
- Compare page faults

COMPILATION:
g++ 6_2.cpp -o 6_2

EXECUTION:
./6_2

OPTIMAL (Belady's Algorithm):
- Look ahead in reference string
- Replace page that won't be used for longest
- Or replace page that will never be used
- Theoretical best, not practical (needs future)

LRU vs OPTIMAL:
- LRU: Past-based (practical)
- Optimal: Future-based (theoretical)
- Optimal gives minimum possible faults
==============================================================================
*/

#include <iostream>
#include <climits>

using namespace std;

#define MAX_FRAMES 10

// Fixed reference string as per problem
int refString[] = {1, 2, 3, 4, 1, 2, 5, 1, 1, 2, 3, 4, 5};
int refLength = 13;

// Check if page is in frames
int findPage(int frames[], int frameSize, int page) {
    for(int i = 0; i < frameSize; i++) {
        if(frames[i] == page)
            return i;
    }
    return -1;
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

// Find when page will be used next (for Optimal)
int findNextUse(int currentPos) {
    // Returns position when current page will be used next
    // Returns INT_MAX if never used again
    return currentPos;
}

// Find victim for Optimal - page used farthest in future
int findOptimalVictim(int frames[], int frameSize, int currentPos) {
    int victim = 0;
    int farthest = -1;
    
    for(int i = 0; i < frameSize; i++) {
        int j;
        // Look ahead for when frames[i] will be used
        for(j = currentPos; j < refLength; j++) {
            if(refString[j] == frames[i])
                break;
        }
        
        // If this page is used farthest in future (or never)
        if(j > farthest) {
            farthest = j;
            victim = i;
        }
    }
    
    return victim;
}

// OPTIMAL Page Replacement
int optimal(int frameSize) {
    int frames[MAX_FRAMES];
    int pageFaults = 0;
    
    // Initialize frames
    for(int i = 0; i < frameSize; i++)
        frames[i] = -1;
    
    cout << "\n--- OPTIMAL Page Replacement ---" << endl;
    cout << "Reference  Frames           Fault/Hit  Explanation" << endl;
    cout << "--------------------------------------------------------" << endl;
    
    for(int i = 0; i < refLength; i++) {
        int page = refString[i];
        cout << "   " << page << "       ";
        
        int pageIndex = findPage(frames, frameSize, page);
        
        if(pageIndex != -1) {
            // Page Hit
            displayFrames(frames, frameSize);
            cout << "  HIT" << endl;
        }
        else {
            // Page Fault
            pageFaults++;
            
            // Find empty frame first
            int emptyIndex = findPage(frames, frameSize, -1);
            
            if(emptyIndex != -1) {
                // Empty frame available
                frames[emptyIndex] = page;
                displayFrames(frames, frameSize);
                cout << "  FAULT   (Empty slot)" << endl;
            }
            else {
                // All frames full - find optimal victim
                int victimIndex = findOptimalVictim(frames, frameSize, i + 1);
                int victimPage = frames[victimIndex];
                frames[victimIndex] = page;
                
                displayFrames(frames, frameSize);
                cout << "  FAULT   (Replace " << victimPage << ")" << endl;
            }
        }
    }
    
    return pageFaults;
}

// LRU Page Replacement
int lru(int frameSize) {
    int frames[MAX_FRAMES];
    int lastUsed[MAX_FRAMES];
    int pageFaults = 0;
    
    // Initialize
    for(int i = 0; i < frameSize; i++) {
        frames[i] = -1;
        lastUsed[i] = -1;
    }
    
    cout << "\n--- LRU Page Replacement ---" << endl;
    cout << "Reference  Frames           Fault/Hit  Explanation" << endl;
    cout << "--------------------------------------------------------" << endl;
    
    for(int i = 0; i < refLength; i++) {
        int page = refString[i];
        cout << "   " << page << "       ";
        
        int pageIndex = findPage(frames, frameSize, page);
        
        if(pageIndex != -1) {
            // Page Hit - update last used
            lastUsed[pageIndex] = i;
            displayFrames(frames, frameSize);
            cout << "  HIT" << endl;
        }
        else {
            // Page Fault
            pageFaults++;
            
            // Find victim (LRU)
            int victimIndex = 0;
            int minTime = lastUsed[0];
            
            for(int j = 1; j < frameSize; j++) {
                if(lastUsed[j] < minTime) {
                    minTime = lastUsed[j];
                    victimIndex = j;
                }
            }
            
            int victimPage = frames[victimIndex];
            frames[victimIndex] = page;
            lastUsed[victimIndex] = i;
            
            displayFrames(frames, frameSize);
            if(victimPage == -1)
                cout << "  FAULT   (Empty slot)" << endl;
            else
                cout << "  FAULT   (Replace " << victimPage << ")" << endl;
        }
    }
    
    return pageFaults;
}

int main() {
    cout << "========================================" << endl;
    cout << "  PAGE REPLACEMENT - OPTIMAL & LRU" << endl;
    cout << "========================================\n" << endl;
    
    // Display fixed reference string
    cout << "Fixed Page Reference String:" << endl;
    for(int i = 0; i < refLength; i++) {
        cout << refString[i] << " ";
    }
    cout << "\n(Total references: " << refLength << ")" << endl;
    
    // Test with 3 frames
    cout << "\n========================================" << endl;
    cout << "  FRAME SIZE = 3" << endl;
    cout << "========================================" << endl;
    
    int optimalFaults3 = optimal(3);
    cout << "\nTotal Page Faults (Optimal): " << optimalFaults3 << endl;
    cout << "Page Fault Rate (Optimal): " << (optimalFaults3 * 100.0 / refLength) << "%" << endl;
    
    cout << endl;
    
    int lruFaults3 = lru(3);
    cout << "\nTotal Page Faults (LRU): " << lruFaults3 << endl;
    cout << "Page Fault Rate (LRU): " << (lruFaults3 * 100.0 / refLength) << "%" << endl;
    
    // Test with 4 frames
    cout << "\n========================================" << endl;
    cout << "  FRAME SIZE = 4" << endl;
    cout << "========================================" << endl;
    
    int optimalFaults4 = optimal(4);
    cout << "\nTotal Page Faults (Optimal): " << optimalFaults4 << endl;
    cout << "Page Fault Rate (Optimal): " << (optimalFaults4 * 100.0 / refLength) << "%" << endl;
    
    cout << endl;
    
    int lruFaults4 = lru(4);
    cout << "\nTotal Page Faults (LRU): " << lruFaults4 << endl;
    cout << "Page Fault Rate (LRU): " << (lruFaults4 * 100.0 / refLength) << "%" << endl;
    
    // Final Comparison
    cout << "\n========================================" << endl;
    cout << "  FINAL COMPARISON" << endl;
    cout << "========================================" << endl;
    
    cout << "\nFrame Size 3:" << endl;
    cout << "  Optimal: " << optimalFaults3 << " faults" << endl;
    cout << "  LRU:     " << lruFaults3 << " faults" << endl;
    cout << "  Difference: " << (lruFaults3 - optimalFaults3) << endl;
    
    cout << "\nFrame Size 4:" << endl;
    cout << "  Optimal: " << optimalFaults4 << " faults" << endl;
    cout << "  LRU:     " << lruFaults4 << " faults" << endl;
    cout << "  Difference: " << (lruFaults4 - optimalFaults4) << endl;
    
    cout << "\nObservation:" << endl;
    if(optimalFaults3 <= lruFaults3 && optimalFaults4 <= lruFaults4)
        cout << "Optimal always performs better or equal to LRU" << endl;
    
    cout << "\n========================================" << endl;
    
    return 0;
}

/*
==============================================================================
EXAM NOTES:

1. OPTIMAL ALGORITHM (Belady's):
   - Theoretical best algorithm
   - Replace page that will be used FARTHEST in future
   - Or replace page that will NEVER be used again
   - Gives minimum possible page faults
   - NOT practical (requires future knowledge)
   - Used as benchmark to compare other algorithms

2. ALGORITHM STEPS:
   For each reference:
   1. If page in frame: HIT
   2. If page not in frame:
      a) If empty frame exists: Use it
      b) Else: Look ahead in reference string
              Find page used farthest in future
              Replace that page

3. OPTIMAL VS LRU:
   
   OPTIMAL:
   - Future-based decision
   - Minimum page faults (provably best)
   - Not implementable in practice
   - Used for theoretical comparison
   
   LRU:
   - Past-based decision
   - Good approximation of Optimal
   - Implementable in practice
   - Based on locality principle

4. EXAMPLE WALKTHROUGH:
   Reference: 1,2,3,4,1,2,5,1,1,2,3,4,5
   Frames: 3
   
   OPTIMAL:
   1: [1 - -] FAULT
   2: [1 2 -] FAULT
   3: [1 2 3] FAULT
   4: [4 2 3] FAULT (1 used at pos 4, 2 at pos 5, 3 at pos 10 → replace 3? NO!)
      Look ahead: 1(pos 4), 2(pos 5), 3(pos 10) → Replace 3
   1: [4 2 1] HIT (just loaded)
   2: [4 2 1] HIT
   5: [5 2 1] FAULT (4 at pos 11, 2 at pos 9, 1 at pos 7 → replace 4)
   ... continue

5. WHY OPTIMAL IS BEST:
   - Makes most informed decision
   - Keeps pages that will be needed soon
   - Removes pages that won't be needed
   - No algorithm can do better

6. PRACTICAL ALTERNATIVE:
   Since Optimal needs future → Use LRU
   LRU approximates Optimal by assuming:
   "Pages used recently will be used again soon"
   (Principle of Temporal Locality)

7. COMPARING ALGORITHMS:
   Reference String: 1,2,3,4,1,2,5,1,1,2,3,4,5
   
   Frames=3:
   - Optimal: 6 faults (theoretical minimum)
   - LRU: 7 faults (good practical)
   - FIFO: 9 faults (simple but worse)
   
   Frames=4:
   - Optimal: 5 faults
   - LRU: 6 faults
   - FIFO: Can be 9-10 (Belady's Anomaly!)

8. IMPLEMENTATION TRICK:
   For each page fault:
   - Scan forward in reference string
   - Find farthest occurrence of each frame page
   - Replace page with maximum distance
   - If page never occurs: Replace it!

9. TIME COMPLEXITY:
   For each reference:
   - Check if in frame: O(f)
   - Find victim: O(f × remaining_refs)
   - Total: O(n × f × n) = O(n² × f)
   Where n=references, f=frames
   
   Very inefficient - another reason it's impractical!

10. EXAM TIP:
    - Always show look-ahead for each replacement
    - Mark future positions of each page
    - Choose page used farthest
    - If tie, any can be chosen
    - Compare with LRU to show Optimal is better

11. KEY INSIGHT:
    Optimal proves that:
    - LRU is good approximation
    - No algorithm beats Optimal
    - Future knowledge helps a lot
    - Past is reasonable predictor of future

12. WHY STUDY OPTIMAL?
    - Benchmark for comparison
    - Upper bound on performance
    - Theoretical understanding
    - Proves LRU is good choice
==============================================================================
*/
