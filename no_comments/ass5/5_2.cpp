#include <iostream>
using namespace std;

int main() {
    int n, m;
    int avail[10], max_m[10][10], alloc[10][10], need[10][10];
    
    cout << "Processes: "; cin >> n;
    cout << "Resources: "; cin >> m;
    
    cout << "Available: ";
    for(int i = 0; i < m; i++) cin >> avail[i];
    
    cout << "Max matrix:\n";
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++) cin >> max_m[i][j];
    
    cout << "Allocation matrix:\n";
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++) cin >> alloc[i][j];
    
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            need[i][j] = max_m[i][j] - alloc[i][j];
    
    int p, req[10];
    cout << "Process number: "; cin >> p;
    cout << "Request: ";
    for(int i = 0; i < m; i++) cin >> req[i];
    
    for(int i = 0; i < m; i++) {
        if(req[i] > need[p][i] || req[i] > avail[i]) {
            cout << "Request DENIED\n";
            return 0;
        }
    }
    
    for(int i = 0; i < m; i++) {
        avail[i] -= req[i];
        alloc[p][i] += req[i];
        need[p][i] -= req[i];
    }
    
    int work[10], finish[10] = {0}, safe[10], count = 0;
    for(int i = 0; i < m; i++) work[i] = avail[i];
    
    while(count < n) {
        bool found = false;
        
        for(int i = 0; i < n; i++) {
            if(finish[i] == 0) {
                bool canRun = true;
                for(int j = 0; j < m; j++) {
                    if(need[i][j] > work[j]) {
                        canRun = false;
                        break;
                    }
                }
                
                if(canRun) {
                    for(int j = 0; j < m; j++)
                        work[j] += alloc[i][j];
                    safe[count++] = i;
                    finish[i] = 1;
                    found = true;
                }
            }
        }
        
        if(!found) {
            cout << "UNSAFE\n";
            cout << "Request DENIED\n";
            return 0;
        }
    }
    
    cout << "SAFE\n";
    cout << "Request GRANTED\n";
    cout << "Safe sequence: ";
    for(int i = 0; i < n; i++)
        cout << "P" << safe[i] << " ";
    cout << "\n";
    
    return 0;
}