#include <iostream>
using namespace std;

void fifo(int ref[], int n, int frames) {
    int mem[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int idx = 0, faults = 0;
    
    cout << "\n=== FIFO ===" << endl;
    cout << "Ref  Frames" << endl;
    
    for(int i = 0; i < n; i++) {
        bool found = false;
        for(int j = 0; j < frames; j++) {
            if(mem[j] == ref[i]) {
                found = true;
                break;
            }
        }
        
        cout << ref[i] << "    ";
        if(!found) {
            mem[idx] = ref[i];
            idx = (idx + 1) % frames;
            faults++;
        }
        
        for(int j = 0; j < frames; j++) {
            if(mem[j] == -1) cout << "- ";
            else cout << mem[j] << " ";
        }
        cout << (found ? " HIT" : " FAULT") << endl;
    }
    cout << "Total Faults: " << faults << endl;
}

void lru(int ref[], int n, int frames) {
    int mem[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int time[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int faults = 0;
    
    cout << "\n=== LRU ===" << endl;
    cout << "Ref  Frames" << endl;
    
    for(int i = 0; i < n; i++) {
        int pos = -1;
        for(int j = 0; j < frames; j++) {
            if(mem[j] == ref[i]) {
                pos = j;
                break;
            }
        }
        
        cout << ref[i] << "    ";
        if(pos != -1) {
            time[pos] = i;
        } else {
            faults++;
            int lru_idx = 0, min_time = time[0];
            for(int j = 1; j < frames; j++) {
                if(time[j] < min_time) {
                    min_time = time[j];
                    lru_idx = j;
                }
            }
            mem[lru_idx] = ref[i];
            time[lru_idx] = i;
        }
        
        for(int j = 0; j < frames; j++) {
            if(mem[j] == -1) cout << "- ";
            else cout << mem[j] << " ";
        }
        cout << (pos != -1 ? " HIT" : " FAULT") << endl;
    }
    cout << "Total Faults: " << faults << endl;
}

int main() {
    int n, frames, ref[50];
    
    cout << "Enter number of references: ";
    cin >> n;
    cout << "Enter number of frames: ";
    cin >> frames;
    
    cout << "Enter page reference string:\n";
    for(int i = 0; i < n; i++) cin >> ref[i];
    
    cout << "\n1. FIFO\n2. LRU\nChoice: ";
    int choice;
    cin >> choice;
    
    if(choice == 1) fifo(ref, n, frames);
    else if(choice == 2) lru(ref, n, frames);
    
    return 0;
}

