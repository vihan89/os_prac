

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
    
    cout << "\nTotal Head Movement: " << totalMovement << " cylinders" << endl;
    cout << "Average Seek Time: " << (totalMovement / (float)(n-1)) << " cylinders" << endl;
    
    return totalMovement;
}

void cscan(int requests[], int n, int head, int diskSize) {
    cout << "\n========================================" << endl;
    cout << "  C-SCAN DISK SCHEDULING" << endl;
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
    

    for(int i = 0; i < rightCount; i++) {
        sequence[seqIndex++] = right[i];
    }
    

    if(rightCount > 0 && right[rightCount-1] != diskSize-1) {
        sequence[seqIndex++] = diskSize - 1;
    }
    

    if(leftCount > 0) {
        sequence[seqIndex++] = 0;
        

        for(int i = 0; i < leftCount; i++) {
            sequence[seqIndex++] = left[i];
        }
    }
    

    calculateMovement(sequence, seqIndex, "C-SCAN");
    
    cout << "\nDirection: Right → End → Jump to Start → Right" << endl;
}

void clook(int requests[], int n, int head, int diskSize) {
    cout << "\n========================================" << endl;
    cout << "  C-LOOK DISK SCHEDULING" << endl;
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
    

    for(int i = 0; i < rightCount; i++) {
        sequence[seqIndex++] = right[i];
    }
    

    if(leftCount > 0) {
        for(int i = 0; i < leftCount; i++) {
            sequence[seqIndex++] = left[i];
        }
    }
    

    calculateMovement(sequence, seqIndex, "C-LOOK");
    
    cout << "\nDirection: Right → Last Request → Jump to First Left → Right" << endl;
}

int main() {
    int n, head, diskSize;
    int requests[MAX_REQUESTS];
    
    cout << "========================================" << endl;
    cout << "  DISK SCHEDULING: C-SCAN & C-LOOK" << endl;
    cout << "========================================\n" << endl;
    

    diskSize = 500;
    head = 85;
    

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
    

    cscan(requests, n, head, diskSize);
    clook(requests, n, head, diskSize);
    

    cout << "\n========================================" << endl;
    cout << "  COMPARISON" << endl;
    cout << "========================================" << endl;
    cout << "C-SCAN: Goes to disk end before jumping" << endl;
    cout << "C-LOOK: Only goes to last request before jumping" << endl;
    cout << "\nC-LOOK is more efficient (less movement)" << endl;
    cout << "Both provide uniform wait time" << endl;
    
    return 0;
}

