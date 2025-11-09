// Banker's Algorithm - Resource Request (Short & Simple)
#include <iostream>
#include <fstream>
using namespace std;

int main() {
    int n, m, avail[10], max_m[10][10], alloc[10][10], need[10][10];
    
    // Read from file (or cin)
    ifstream fin("state.txt");
    if(!fin) { cout << "File not found. Using keyboard input.\n"; }
    istream& in = fin.is_open() ? (istream&)fin : cin;
    
    in >> n >> m;
    for(int i = 0; i < m; i++) in >> avail[i];
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++) in >> max_m[i][j];
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++) in >> alloc[i][j];
    
    // Calculate Need
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            need[i][j] = max_m[i][j] - alloc[i][j];
    
    // Get request
    int p, req[10];
    cout << "Process (0-" << n-1 << "): "; cin >> p;
    cout << "Request: ";
    for(int i = 0; i < m; i++) cin >> req[i];
    
    // Check validity
    for(int i = 0; i < m; i++) {
        if(req[i] > need[p][i] || req[i] > avail[i]) {
            cout << "DENIED: Invalid or unavailable\n";
            return 0;
        }
    }
    
    // Pretend grant
    for(int i = 0; i < m; i++) {
        avail[i] -= req[i];
        alloc[p][i] += req[i];
        need[p][i] -= req[i];
    }
    
    // Safety check
    int work[10], finish[10] = {0}, safe[10], cnt = 0;
    for(int i = 0; i < m; i++) work[i] = avail[i];
    
    while(cnt < n) {
        bool found = false;
        for(int i = 0; i < n; i++) {
            if(!finish[i]) {
                bool ok = true;
                for(int j = 0; j < m; j++)
                    if(need[i][j] > work[j]) { ok = false; break; }
                
                if(ok) {
                    for(int j = 0; j < m; j++) work[j] += alloc[i][j];
                    safe[cnt++] = i;
                    finish[i] = 1;
                    found = true;
                }
            }
        }
        
        if(!found) {
            cout << "UNSAFE: Request DENIED\n";
            return 0;
        }
    }
    
    cout << "SAFE: Request GRANTED\nSequence: ";
    for(int i = 0; i < n; i++) cout << "P" << safe[i] << " ";
    cout << "\n";
    
    return 0;
}