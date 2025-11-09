/*
==============================================================================
ASSIGNMENT 8.1: DISK SCHEDULING - C-SCAN & C-LOOK
==============================================================================

PROBLEM:
- Disk: 0-499
- Initial head position: 85
- Implement C-SCAN (Circular SCAN)
- Implement C-LOOK (Circular LOOK)
- Calculate total head movement and average seek time

COMPILATION:
g++ 8_1.cpp -o 8_1

EXECUTION:
./8_1

C-SCAN (Circular SCAN):
- Head moves towards one end
- Services requests while moving
- Reaches end, jumps to other end
- Continues in same direction
- More uniform wait time than SCAN

C-LOOK (Circular LOOK):
- Like C-SCAN but doesn't go to end
- Goes to last request in direction
- Then jumps to first request on other end
- More efficient than C-SCAN
==============================================================================
*/

#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

#define MAX_REQUESTS 50

// Display sequence and calculate head movement
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
    
    cout << "\nTotal Head Movement: " << totalMovement << " cylinders" << endl;
    cout << "Average Seek Time: " << (totalMovement / (float)(n-1)) << " cylinders" << endl;
    
    return totalMovement;
}

// C-SCAN Algorithm
void cscan(int requests[], int n, int head, int diskSize) {
    cout << "\n========================================" << endl;
    cout << "  C-SCAN DISK SCHEDULING" << endl;
    cout << "========================================" << endl;
    
    int sequence[MAX_REQUESTS + 3];
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int leftCount = 0, rightCount = 0;
    
    // Separate requests into left and right of head
    for(int i = 0; i < n; i++) {
        if(requests[i] < head)
            left[leftCount++] = requests[i];
        else
            right[rightCount++] = requests[i];
    }
    
    // Sort both arrays
    sort(left, left + leftCount);
    sort(right, right + rightCount);
    
    int seqIndex = 0;
    
    // Start from head position
    sequence[seqIndex++] = head;
    
    // Move right towards end (0 to diskSize-1)
    // Service all requests on right
    for(int i = 0; i < rightCount; i++) {
        sequence[seqIndex++] = right[i];
    }
    
    // Reach end of disk
    if(rightCount > 0 && right[rightCount-1] != diskSize-1) {
        sequence[seqIndex++] = diskSize - 1;
    }
    
    // Jump to beginning (0)
    if(leftCount > 0) {
        sequence[seqIndex++] = 0;
        
        // Service requests from left (in ascending order)
        for(int i = 0; i < leftCount; i++) {
            sequence[seqIndex++] = left[i];
        }
    }
    
    // Display and calculate
    calculateMovement(sequence, seqIndex, "C-SCAN");
    
    cout << "\nDirection: Right → End → Jump to Start → Right" << endl;
}

// C-LOOK Algorithm
void clook(int requests[], int n, int head, int diskSize) {
    cout << "\n========================================" << endl;
    cout << "  C-LOOK DISK SCHEDULING" << endl;
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
    
    // Start from head
    sequence[seqIndex++] = head;
    
    // Move right to last request (not to end)
    for(int i = 0; i < rightCount; i++) {
        sequence[seqIndex++] = right[i];
    }
    
    // Jump to first request on left
    // Then service in ascending order
    if(leftCount > 0) {
        for(int i = 0; i < leftCount; i++) {
            sequence[seqIndex++] = left[i];
        }
    }
    
    // Display and calculate
    calculateMovement(sequence, seqIndex, "C-LOOK");
    
    cout << "\nDirection: Right → Last Request → Jump to First Left → Right" << endl;
}

int main() {
    int n, head, diskSize;
    int requests[MAX_REQUESTS];
    
    cout << "========================================" << endl;
    cout << "  DISK SCHEDULING: C-SCAN & C-LOOK" << endl;
    cout << "========================================\n" << endl;
    
    // Fixed parameters as per problem statement
    diskSize = 500;  // Disk: 0-499
    head = 85;       // Initial head position
    
    // Fixed request queue from problem statement
    int defaultRequests[] = {10, 229, 39, 400, 18, 145, 120, 480, 20, 250};
    n = 10;
    
    cout << "Using problem statement values:" << endl;
    cout << "Disk Size: 0-" << (diskSize-1) << endl;
    cout << "Initial Head Position: " << head << endl;
    cout << "Request Queue (FIFO): ";
    for(int i = 0; i < n; i++) {
        requests[i] = defaultRequests[i];
        cout << requests[i] << " ";
    }
    cout << "\nTotal Requests: " << n << endl;
    cout << "Initial Direction: Towards 499 (right)" << endl;
    
    // Run algorithms
    cscan(requests, n, head, diskSize);
    clook(requests, n, head, diskSize);
    
    // Comparison
    cout << "\n========================================" << endl;
    cout << "  COMPARISON" << endl;
    cout << "========================================" << endl;
    cout << "C-SCAN: Goes to disk end before jumping" << endl;
    cout << "C-LOOK: Only goes to last request before jumping" << endl;
    cout << "\nC-LOOK is more efficient (less movement)" << endl;
    cout << "Both provide uniform wait time" << endl;
    
    return 0;
}

/*
==============================================================================
EXAM NOTES:

1. DISK SCHEDULING PURPOSE:
   - Minimize seek time (head movement)
   - Improve throughput
   - Reduce average response time
   - Prevent starvation

2. C-SCAN (Circular SCAN):
   
   Algorithm:
   1. Move in one direction (e.g., towards higher)
   2. Service all requests in that direction
   3. Reach end of disk
   4. Jump to beginning (0)
   5. Continue servicing in same direction
   
   Characteristics:
   - Circular version of SCAN
   - More uniform wait time than SCAN
   - No direction reversal
   - Better for heavy load

3. C-LOOK (Circular LOOK):
   
   Algorithm:
   1. Move in one direction
   2. Service requests until last request
   3. Jump to first request on other end
   4. Continue in same direction
   
   Difference from C-SCAN:
   - Doesn't go to disk end
   - Only goes to last request
   - More efficient

4. EXAMPLE EXECUTION:
   Disk: 0-499
   Head: 85
   Queue: 170, 50, 25, 150, 130
   
   C-SCAN (moving right):
   85 → 130 → 150 → 170 → 499 → 0 → 25 → 50
   Movement: 45+20+20+329+499+25+25 = 963
   
   C-LOOK (moving right):
   85 → 130 → 150 → 170 → 25 → 50
   Movement: 45+20+20+145+25 = 255

5. FORMULAS:
   Total Head Movement = Σ|current - next|
   Average Seek Time = Total Movement / (n-1)
   Where n = number of positions in sequence

6. DIRECTION CHOICE:
   Can start moving:
   - Towards 0 (left)
   - Towards end (right)
   
   Usually move towards closer end
   Or based on last direction

7. ADVANTAGES:
   C-SCAN:
   ✓ Uniform wait time
   ✓ No starvation
   ✓ Better for heavy I/O
   ✓ Predictable service
   
   C-LOOK:
   ✓ More efficient than C-SCAN
   ✓ No unnecessary movement to end
   ✓ Faster average response

8. DISADVANTAGES:
   - May not be optimal
   - Long jump from end to beginning
   - Some requests may wait full cycle

9. COMPARISON WITH OTHER ALGORITHMS:

   FCFS: Process in arrival order
   - Fair but inefficient
   
   SCAN: Go to end, reverse, repeat
   - Elevator algorithm
   - Middle cylinders favored
   
   C-SCAN: Go to end, jump to start
   - More uniform than SCAN
   
   C-LOOK: Go to last, jump to first
   - Most efficient circular

10. IMPLEMENTATION STEPS:
    1. Separate requests into left and right of head
    2. Sort both arrays
    3. Service one direction (e.g., right)
    4. Jump to other end
    5. Service remaining requests

11. EXAM CALCULATION:
    Show:
    1. Initial position
    2. Complete sequence with arrows
    3. Movement between each step
    4. Total movement
    5. Average seek time

12. EXAMPLE PROBLEM:
    Q: Head at 85, requests: 170,50,25,150,130
       Disk: 0-499, use C-SCAN moving right
    
    A: Right of 85: 130,150,170 (sorted)
       Left of 85: 25,50 (sorted)
       
       Sequence:
       85→130(+45)→150(+20)→170(+20)→499(+329)
       →0(+499)→25(+25)→50(+25)
       
       Total: 963 cylinders

13. C-SCAN vs SCAN:
    SCAN (Elevator):
    0←···←85←···←200
    0→···→200
    - Reverses direction
    - Middle favored
    
    C-SCAN:
    85→···→499→[jump]→0→···
    - Always same direction
    - Uniform service

14. EXAM TIP:
    - Draw diagram showing head movement
    - Mark requests on number line
    - Show jump clearly
    - Calculate step-by-step
    - Compare with other algorithms

15. REAL WORLD:
    - Used in hard disk drives
    - Similar to elevator scheduling
    - Modern SSDs use different algorithms
    - RAID systems use variations

16. TIME COMPLEXITY:
    - Sorting: O(n log n)
    - Traversal: O(n)
    - Total: O(n log n)

17. BEST/WORST CASE:
    Best: All requests in one direction
    Worst: Requests spread across disk
==============================================================================
*/
