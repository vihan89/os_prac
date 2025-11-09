// C-SCAN / C-LOOK Disk Scheduling - Compare both algorithms
#include <bits/stdc++.h>
using namespace std;

int disk_schedule(int req[], int n, int head, int size, bool cscan, string name) {
    int L[50], R[50], lc = 0, rc = 0;
    
    // Split into left and right
    for(int i = 0; i < n; i++) {
        if(req[i] < head)
            L[lc++] = req[i];
        else
            R[rc++] = req[i];
    }
    
    sort(L, L + lc);
    sort(R, R + rc);
    
    // Build sequence: head -> right -> (end/jump) -> left
    int seq[100], sc = 0;
    seq[sc++] = head;
    for(int i = 0; i < rc; i++) seq[sc++] = R[i];
    
    if(cscan) {
        if(rc > 0 && seq[sc-1] != size-1) seq[sc++] = size-1;
        seq[sc++] = 0;
    }
    
    for(int i = 0; i < lc; i++) seq[sc++] = L[i];
    
    // Calculate and display
    cout << "\n" << name << ":\n";
    cout << "Sequence: ";
    int total = 0;
    for(int i = 0; i < sc; i++) {
        cout << seq[i];
        if(i < sc-1) cout << " -> ";
        if(i) total += abs(seq[i] - seq[i-1]);
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
    disk_schedule(req, n, head, size, true, "C-SCAN");
    disk_schedule(req, n, head, size, false, "C-LOOK");
    
    return 0;
}