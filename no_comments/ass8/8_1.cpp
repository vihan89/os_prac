#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

void cscan(int req[], int n, int head, int size) {
    int left[50], right[50], lc = 0, rc = 0;
    
    for(int i = 0; i < n; i++) {
        if(req[i] < head) left[lc++] = req[i];
        else right[rc++] = req[i];
    }
    
    sort(left, left + lc);
    sort(right, right + rc);
    
    cout << "\n=== C-SCAN ===" << endl;
    cout << "Sequence: " << head;
    int total = 0, prev = head;
    
    for(int i = 0; i < rc; i++) {
        cout << " -> " << right[i];
        total += abs(right[i] - prev);
        prev = right[i];
    }
    if(rc > 0 && right[rc-1] != size-1) {
        cout << " -> " << (size-1);
        total += abs(size-1 - prev);
        prev = size-1;
    }
    if(lc > 0) {
        cout << " -> 0";
        total += abs(prev - 0);
        prev = 0;
        for(int i = 0; i < lc; i++) {
            cout << " -> " << left[i];
            total += abs(left[i] - prev);
            prev = left[i];
        }
    }
    cout << "\nTotal Movement: " << total << endl;
}

void clook(int req[], int n, int head, int size) {
    int left[50], right[50], lc = 0, rc = 0;
    
    for(int i = 0; i < n; i++) {
        if(req[i] < head) left[lc++] = req[i];
        else right[rc++] = req[i];
    }
    
    sort(left, left + lc);
    sort(right, right + rc);
    
    cout << "\n=== C-LOOK ===" << endl;
    cout << "Sequence: " << head;
    int total = 0, prev = head;
    
    for(int i = 0; i < rc; i++) {
        cout << " -> " << right[i];
        total += abs(right[i] - prev);
        prev = right[i];
    }
    if(lc > 0) {
        cout << " -> " << left[0];
        total += abs(left[0] - prev);
        prev = left[0];
        for(int i = 1; i < lc; i++) {
            cout << " -> " << left[i];
            total += abs(left[i] - prev);
            prev = left[i];
        }
    }
    cout << "\nTotal Movement: " << total << endl;
}

int main() {
    int n, head, size, req[50];
    
    cout << "Enter disk size: ";
    cin >> size;
    cout << "Enter initial head position: ";
    cin >> head;
    cout << "Enter number of requests: ";
    cin >> n;
    cout << "Enter requests:\n";
    for(int i = 0; i < n; i++) cin >> req[i];
    
    cout << "\n1. C-SCAN\n2. C-LOOK\nChoice: ";
    int choice;
    cin >> choice;
    
    if(choice == 1) cscan(req, n, head, size);
    else if(choice == 2) clook(req, n, head, size);
    
    return 0;
}