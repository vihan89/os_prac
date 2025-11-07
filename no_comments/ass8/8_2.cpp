#include <iostream>
#include <algorithm>
using namespace std;

void disk_schedule(int req[], int n, int head, int size, bool scan) {
    int left[50], right[50], lc = 0, rc = 0;
    
    for(int i = 0; i < n; i++)
        req[i] < head ? left[lc++] = req[i] : right[rc++] = req[i];
    
    sort(left, left + lc);
    sort(right, right + rc);
    
    cout << "\nSequence: " << head;
    int total = 0, prev = head;
    
    for(int i = 0; i < rc; i++) {
        cout << " -> " << right[i];
        total += right[i] - prev;
        prev = right[i];
    }
    
    if(scan && (rc == 0 || prev != size-1)) {
        cout << " -> " << (size-1);
        total += size-1 - prev;
        prev = size-1;
    }
    
    for(int i = lc-1; i >= 0; i--) {
        cout << " -> " << left[i];
        total += prev - left[i];
        prev = left[i];
    }
    
    cout << "\nTotal: " << total << endl;
}

int main() {
    int n, head, size, req[50];
    cout << "Disk size: "; cin >> size;
    cout << "Head position: "; cin >> head;
    cout << "Requests: "; cin >> n;
    for(int i = 0; i < n; i++) cin >> req[i];
    cout << "1.SCAN 2.LOOK: ";
    int ch; cin >> ch;
    disk_schedule(req, n, head, size, ch == 1);
    return 0;
}