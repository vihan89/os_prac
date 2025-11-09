/*
==============================================================================
ASSIGNMENT 8.2: DISK SCHEDULING - SCAN & LOOK
==============================================================================

PROBLEM:
- Disk: 0-499
- Initial head position: 185
- Implement SCAN (Elevator Algorithm)
- Implement LOOK
- Calculate total head movement and average seek time

COMPILATION:
g++ 8_2.cpp -o 8_2

EXECUTION:
./8_2

SCAN (Elevator):
- Head moves in one direction
- Services all requests until end
- Reverses direction
- Services requests on return
- Like elevator in building

LOOK:
- Like SCAN but doesn't go to end
- Reverses at last request
- More efficient than SCAN
==============================================================================
*/

#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

#define MAX_REQUESTS 50

// Calculate and display head movement
int calculateMovement(int sequence[], int n, string name) {
    cout << "\n" << name << " Sequence:" << endl;
    
    int totalMovement = 0;
    
    for(int i = 0; i < n; i++) {
        cout << sequence[i];
        if(i < n-1) {
            cout << " -> ";
            totalMovement += abs(sequence[i+1] - sequence[i]);
        }
    }
    cout << endl;
    
    cout << "\nDetailed Movement:" << endl;
    for(int i = 0; i < n-1; i++) {
        int move = abs(sequence[i+1] - sequence[i]);
        cout << sequence[i] << " → " << sequence[i+1] 
             << " : " << move << " cylinders" << endl;
    }
    
    cout << "\nTotal Head Movement: " << totalMovement << " cylinders" << endl;
    cout << "Average Seek Time: " << (totalMovement / (float)(n-1)) << " cylinders" << endl;
    
    return totalMovement;
}

// SCAN Algorithm
void scan(int requests[], int n, int head, int diskSize, string direction) {
    cout << "\n========================================" << endl;
    cout << "  SCAN (ELEVATOR) DISK SCHEDULING" << endl;
    cout << "========================================" << endl;
    
    int sequence[MAX_REQUESTS + 3];
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int leftCount = 0, rightCount = 0;
    
    // Separate requests
    for(int i = 0; i < n; i++) {
        if(requests[i] < head)
            left[leftCount++] = requests[i];
        else
            right[rightCount++] = requests[i];
    }
    
    // Sort
    sort(left, left + leftCount);
    sort(right, right + rightCount);
    
    int seqIndex = 0;
    sequence[seqIndex++] = head;
    
    if(direction == "right" || direction == "high") {
        // Move right first
        // Service all right requests
        for(int i = 0; i < rightCount; i++) {
            sequence[seqIndex++] = right[i];
        }
        
        // Go to end of disk
        if(rightCount == 0 || right[rightCount-1] != diskSize-1) {
            sequence[seqIndex++] = diskSize - 1;
        }
        
        // Reverse and service left requests (descending)
        for(int i = leftCount - 1; i >= 0; i--) {
            sequence[seqIndex++] = left[i];
        }
        
        cout << "Direction: Right → End → Reverse → Left" << endl;
    }
    else {
        // Move left first
        // Service all left requests (descending)
        for(int i = leftCount - 1; i >= 0; i--) {
            sequence[seqIndex++] = left[i];
        }
        
        // Go to beginning
        if(leftCount == 0 || left[0] != 0) {
            sequence[seqIndex++] = 0;
        }
        
        // Reverse and service right requests
        for(int i = 0; i < rightCount; i++) {
            sequence[seqIndex++] = right[i];
        }
        
        cout << "Direction: Left → Start → Reverse → Right" << endl;
    }
    
    calculateMovement(sequence, seqIndex, "SCAN");
}

// LOOK Algorithm
void look(int requests[], int n, int head, int diskSize, string direction) {
    cout << "\n========================================" << endl;
    cout << "  LOOK DISK SCHEDULING" << endl;
    cout << "========================================" << endl;
    
    int sequence[MAX_REQUESTS + 1];
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int leftCount = 0, rightCount = 0;
    
    // Separate requests
    for(int i = 0; i < n; i++) {
        if(requests[i] < head)
            left[leftCount++] = requests[i];
        else
            right[rightCount++] = requests[i];
    }
    
    // Sort
    sort(left, left + leftCount);
    sort(right, right + rightCount);
    
    int seqIndex = 0;
    sequence[seqIndex++] = head;
    
    if(direction == "right" || direction == "high") {
        // Move right to last request (not to end)
        for(int i = 0; i < rightCount; i++) {
            sequence[seqIndex++] = right[i];
        }
        
        // Reverse at last request
        // Service left requests (descending)
        for(int i = leftCount - 1; i >= 0; i--) {
            sequence[seqIndex++] = left[i];
        }
        
        cout << "Direction: Right → Last → Reverse → Left" << endl;
    }
    else {
        // Move left to first request
        for(int i = leftCount - 1; i >= 0; i--) {
            sequence[seqIndex++] = left[i];
        }
        
        // Reverse at first request
        // Service right requests
        for(int i = 0; i < rightCount; i++) {
            sequence[seqIndex++] = right[i];
        }
        
        cout << "Direction: Left → First → Reverse → Right" << endl;
    }
    
    calculateMovement(sequence, seqIndex, "LOOK");
}

int main() {
    int n, head, diskSize;
    int requests[MAX_REQUESTS];
    string direction;
    
    cout << "========================================" << endl;
    cout << "  DISK SCHEDULING: SCAN & LOOK" << endl;
    cout << "========================================\n" << endl;
    
    // Fixed parameters as per problem statement
    diskSize = 500;  // Disk: 0-499
    head = 185;      // Initial head position
    direction = "right";  // Initial movement towards 499
    
    // Fixed request queue from problem statement
    int defaultRequests[] = {20, 229, 39, 450, 18, 145, 120, 380, 20, 250};
    n = 10;
    
    cout << "Using problem statement values:" << endl;
    cout << "Disk Size: 0-" << (diskSize-1) << endl;
    cout << "Initial Head Position: " << head << endl;
    cout << "Initial Direction: " << direction << " (towards 499)" << endl;
    cout << "Request Queue (FIFO): ";
    for(int i = 0; i < n; i++) {
        requests[i] = defaultRequests[i];
        cout << requests[i] << " ";
    }
    cout << "\nTotal Requests: " << n << endl;
    
    // Run algorithms
    scan(requests, n, head, diskSize, direction);
    look(requests, n, head, diskSize, direction);
    
    // Comparison
    cout << "\n========================================" << endl;
    cout << "  COMPARISON: SCAN vs LOOK" << endl;
    cout << "========================================" << endl;
    cout << "\nSCAN:" << endl;
    cout << "  ✓ Goes to disk end" << endl;
    cout << "  ✓ Then reverses direction" << endl;
    cout << "  ✗ Unnecessary movement if no requests at end" << endl;
    cout << "  - Known as 'Elevator Algorithm'" << endl;
    
    cout << "\nLOOK:" << endl;
    cout << "  ✓ Only goes to last request" << endl;
    cout << "  ✓ Then reverses direction" << endl;
    cout << "  ✓ More efficient than SCAN" << endl;
    cout << "  ✓ No unnecessary movement" << endl;
    
    cout << "\nConclusion: LOOK is generally better!" << endl;
    
    return 0;
}

/*
==============================================================================
EXAM NOTES:

1. SCAN (Elevator Algorithm):
   
   Algorithm:
   1. Start from current head position
   2. Move in initial direction
   3. Service all requests in that direction
   4. Reach end of disk (0 or diskSize-1)
   5. Reverse direction
   6. Service remaining requests
   
   Name: Called "Elevator" because it works like building elevator
   - Goes to top floor, then comes down
   - Services requests in both directions

2. LOOK Algorithm:
   
   Algorithm:
   1. Start from current head position
   2. Move in initial direction
   3. Service requests until LAST request in that direction
   4. Reverse direction (don't go to end)
   5. Service remaining requests
   
   Difference: Only goes to last request, not disk end

3. EXAMPLE EXECUTION:
   Disk: 0-499
   Head: 185
   Queue: 300, 90, 50, 150, 270, 450, 130
   Direction: Right
   
   SCAN (moving right):
   185 → 270 → 300 → 450 → 499 → 150 → 130 → 90 → 50
   Movement: 85+30+150+49+349+20+40+40 = 763
   
   LOOK (moving right):
   185 → 270 → 300 → 450 → 150 → 130 → 90 → 50
   Movement: 85+30+150+300+20+40+40 = 665

4. FORMULAS:
   Total Seek Time = Σ|current - next|
   Average Seek Time = Total / (n-1)
   
   Where n = total positions (including initial)

5. DIRECTION DETERMINATION:
   Method 1: Given in problem
   Method 2: Based on last movement
   Method 3: Towards nearest end
   Method 4: Towards denser cluster

6. SCAN VARIANTS:
   SCAN: Go to end, reverse
   C-SCAN: Go to end, jump to start
   LOOK: Go to last request, reverse
   C-LOOK: Go to last, jump to first

7. ADVANTAGES:

   SCAN:
   ✓ Better than FCFS
   ✓ Prevents starvation
   ✓ Predictable
   ✓ Good for moderate load
   
   LOOK:
   ✓ More efficient than SCAN
   ✓ No wasted movement
   ✓ Faster average response
   ✓ Still prevents starvation

8. DISADVANTAGES:

   SCAN:
   ✗ Middle cylinders favored
   ✗ Unnecessary movement to end
   ✗ Wait time varies
   
   LOOK:
   ✗ Still favors middle cylinders
   ✗ Not optimal
   ✗ Implementation complex

9. WHEN TO USE:
   
   Use SCAN when:
   - Uniform distribution of requests
   - Moderate I/O load
   - Predictability important
   
   Use LOOK when:
   - Want better performance
   - Requests not at disk ends
   - Efficiency critical

10. COMPARISON TABLE:
    
    Algorithm  | Movement | Wait Time | Fairness
    -----------|----------|-----------|----------
    FCFS       | High     | Variable  | Perfect
    SCAN       | Medium   | Variable  | Good
    LOOK       | Low      | Variable  | Good
    C-SCAN     | Medium   | Uniform   | Better
    C-LOOK     | Low      | Uniform   | Better

11. EXAM CALCULATION:
    Must show:
    1. Separate into left/right of head
    2. Sort both arrays
    3. Determine initial direction
    4. Service first direction
    5. Go to end (SCAN) or last (LOOK)
    6. Reverse and service other direction
    7. Calculate total movement

12. IMPLEMENTATION TRICK:
    Left array: Sort ascending, traverse descending
    Right array: Sort ascending, traverse ascending
    This gives correct order for both directions

13. EXAMPLE PROBLEM:
    Q: Head at 185, requests: 300,90,50,150,270,450,130
       Disk: 0-499, SCAN moving right
    
    A: Right of 185: 270,300,450 (sorted)
       Left of 185: 50,90,130,150 (sorted)
       
       Sequence:
       185→270→300→450→499→150→130→90→50
       
       Movement:
       85+30+150+49 (to end)
       +349+20+40+40 (reverse)
       = 763 cylinders

14. SCAN vs C-SCAN:
    
    SCAN:
    →→→→→ [end] ←←←←←
    - Bidirectional
    - Services while reversing
    
    C-SCAN:
    →→→→→ [end] [jump] →→→→→
    - Unidirectional
    - Jump without servicing

15. EXAM TIP:
    - Draw number line with requests
    - Mark head position
    - Show movement with arrows
    - Write movement at each step
    - Sum for total
    - Compare algorithms

16. REAL WORLD:
    - Traditional hard disk drives
    - Elevator scheduling in buildings
    - Print queue management
    - Network packet scheduling

17. MODERN SYSTEMS:
    - SSDs don't use these (no seek time)
    - HDDs still use variations
    - Linux I/O scheduler uses deadline/CFQ
    - Windows uses anticipatory scheduling

18. TIME COMPLEXITY:
    - Sorting: O(n log n)
    - Service: O(n)
    - Total: O(n log n)

19. PERFORMANCE METRICS:
    - Throughput: Requests/second
    - Response Time: Wait + Service
    - Fairness: Variance in wait time
    - Seek Time: Head movement

20. BEST/WORST CASE:
    
    Best Case:
    - All requests in one direction
    - No reversal needed
    
    Worst Case:
    - Requests at both extremes
    - Head in middle
    - Maximum travel distance
==============================================================================
*/
