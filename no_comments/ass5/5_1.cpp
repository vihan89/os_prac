#include <iostream>
using namespace std;

int n, m;
int avail[10], max_m[10][10], alloc[10][10], need[10][10];

void calcNeed() {
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            need[i][j] = max_m[i][j] - alloc[i][j];
}

bool isSafe(int safe[]) {
    int work[10], finish[10] = {0}, count = 0;
    for(int i = 0; i < m; i++) work[i] = avail[i];
    
    while(count < n) {
        bool found = false;
        for(int i = 0; i < n; i++) {
            if(!finish[i]) {
                bool can = true;
                for(int j = 0; j < m; j++)
                    if(need[i][j] > work[j]) { can = false; break; }
                
                if(can) {
                    for(int j = 0; j < m; j++) work[j] += alloc[i][j];
                    safe[count++] = i;
                    finish[i] = 1;
                    found = true;
                }
            }
        }
        if(!found) return false;
    }
    return true;
}

int main() {
    cout << "Enter processes, resources: ";
    cin >> n >> m;
    
    cout << "Enter Available:\n";
    for(int i = 0; i < m; i++) cin >> avail[i];
    
    cout << "Enter Max matrix:\n";
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++) cin >> max_m[i][j];
    
    cout << "Enter Allocation matrix:\n";
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++) cin >> alloc[i][j];
    
    calcNeed();
    
    int safe[10];
    if(isSafe(safe)) {
        cout << "SAFE. Sequence: ";
        for(int i = 0; i < n; i++) cout << "P" << safe[i] << " ";
        cout << endl;
    } else {
        cout << "UNSAFE" << endl;
    }
    
    return 0;
}