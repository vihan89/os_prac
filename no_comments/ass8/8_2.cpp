

#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

#define MAX_REQUESTS 50

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

void scan(int requests[], int n, int head, int diskSize, string direction) {
    cout << "\n========================================" << endl;
    cout << "  SCAN (ELEVATOR) DISK SCHEDULING" << endl;
    cout << "========================================" << endl;
    
    int sequence[MAX_REQUESTS + 3];
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int leftCount = 0, rightCount = 0;
    

    for(int i = 0; i < n; i++) {
        if(requests[i] < head)
            left[leftCount++] = requests[i];
        else
            right[rightCount++] = requests[i];
    }
    

    sort(left, left + leftCount);
    sort(right, right + rightCount);
    
    int seqIndex = 0;
    sequence[seqIndex++] = head;
    
    if(direction == "right" || direction == "high") {

        for(int i = 0; i < rightCount; i++) {
            sequence[seqIndex++] = right[i];
        }
        

        if(rightCount == 0 || right[rightCount-1] != diskSize-1) {
            sequence[seqIndex++] = diskSize - 1;
        }
        

        for(int i = leftCount - 1; i >= 0; i--) {
            sequence[seqIndex++] = left[i];
        }
        
        cout << "Direction: Right → End → Reverse → Left" << endl;
    }
    else {

        for(int i = leftCount - 1; i >= 0; i--) {
            sequence[seqIndex++] = left[i];
        }
        

        if(leftCount == 0 || left[0] != 0) {
            sequence[seqIndex++] = 0;
        }
        

        for(int i = 0; i < rightCount; i++) {
            sequence[seqIndex++] = right[i];
        }
        
        cout << "Direction: Left → Start → Reverse → Right" << endl;
    }
    
    calculateMovement(sequence, seqIndex, "SCAN");
}

void look(int requests[], int n, int head, int diskSize, string direction) {
    cout << "\n========================================" << endl;
    cout << "  LOOK DISK SCHEDULING" << endl;
    cout << "========================================" << endl;
    
    int sequence[MAX_REQUESTS + 1];
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int leftCount = 0, rightCount = 0;
    

    for(int i = 0; i < n; i++) {
        if(requests[i] < head)
            left[leftCount++] = requests[i];
        else
            right[rightCount++] = requests[i];
    }
    

    sort(left, left + leftCount);
    sort(right, right + rightCount);
    
    int seqIndex = 0;
    sequence[seqIndex++] = head;
    
    if(direction == "right" || direction == "high") {

        for(int i = 0; i < rightCount; i++) {
            sequence[seqIndex++] = right[i];
        }
        

        for(int i = leftCount - 1; i >= 0; i--) {
            sequence[seqIndex++] = left[i];
        }
        
        cout << "Direction: Right → Last → Reverse → Left" << endl;
    }
    else {

        for(int i = leftCount - 1; i >= 0; i--) {
            sequence[seqIndex++] = left[i];
        }
        

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
    

    diskSize = 500;
    head = 185;
    direction = "right";
    

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
    

    scan(requests, n, head, diskSize, direction);
    look(requests, n, head, diskSize, direction);
    

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

