// SCAN / LOOK Disk Scheduling - Compare both algorithms
#include <iostream>
#include <algorithm>
using namespace std;

int disk_schedule(int req[], int n, int head, int size, bool scan, string name) {
    int left[50], right[50], lc = 0, rc = 0;
    
    // Split into left and right of head
    for(int i = 0; i < n; i++) {
        if(req[i] < head)
            left[lc++] = req[i];
        else
            right[rc++] = req[i];
    }
    
    sort(left, left + lc);
    sort(right, right + rc);
    
    cout << "\n" << name << ":\n";
    cout << "Sequence: " << head;
    int total = 0, prev = head;
    
    // Go right first (towards 499)
    for(int i = 0; i < rc; i++) {
        cout << " -> " << right[i];
        total += right[i] - prev;
        prev = right[i];
    }
    
    // SCAN: go to end, LOOK: don't go to end
    if(scan && (rc == 0 || prev != size-1)) {
        cout << " -> " << (size-1);
        total += size-1 - prev;
        prev = size-1;
    }
    
    // Reverse direction and serve left requests
    for(int i = lc-1; i >= 0; i--) {
        cout << " -> " << left[i];
        total += prev - left[i];
        prev = left[i];
    }
    
    cout << "\nTotal Head Movement: " << total;
    cout << "\nAverage Seek Distance: " << (float)total/n << "\n";
    
    return total;
}

int main() {
    int n, head, size, req[50];
    cout << "Disk size (0 to ?): "; cin >> size;
    cout << "Head position: "; cin >> head;
    cout << "Number of requests: "; cin >> n;
    cout << "Enter requests: ";
    for(int i = 0; i < n; i++) cin >> req[i];
    
    // Run both algorithms
    disk_schedule(req, n, head, size, true, "SCAN");
    disk_schedule(req, n, head, size, false, "LOOK");
    
    return 0;
}