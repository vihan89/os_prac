#include <iostream>
#include <algorithm>
using namespace std;

void disk_schedule(int req[], int n, int head, int size, bool cscan) {
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
    
    if(cscan && rc > 0 && prev != size-1) {
        cout << " -> " << (size-1);
        total += size-1 - prev;
        prev = size-1;
    }
    
    if(lc > 0) {
        int jump = cscan ? 0 : left[0];
        cout << " -> " << jump;
        total += abs(prev - jump);
        prev = jump;
        
        for(int i = (cscan ? 0 : 1); i < lc; i++) {
            cout << " -> " << left[i];
            total += left[i] - prev;
            prev = left[i];
        }
    }
    
    cout << "\nTotal: " << total << endl;
}

int main() {
    int n, head, size, req[50];
    cout << "Disk size: "; cin >> size;
    cout << "Head position: "; cin >> head;
    cout << "Requests: "; cin >> n;
    for(int i = 0; i < n; i++) cin >> req[i];
    cout << "1.C-SCAN 2.C-LOOK: ";
    int ch; cin >> ch;
    disk_schedule(req, n, head, size, ch == 1);
    return 0;
}