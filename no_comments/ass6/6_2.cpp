#include <iostream>
using namespace std;

int page_ref[50], n;

void optimal(int frames) {
    int mem[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int faults = 0;
    
    cout << "\n=== OPTIMAL ===" << endl;
    cout << "Ref  Frames" << endl;
    
    for(int i = 0; i < n; i++) {
        int pos = -1;
        for(int j = 0; j < frames; j++) {
            if(mem[j] == page_ref[i]) {
                pos = j;
                break;
            }
        }
        
        cout << page_ref[i] << "    ";
        if(pos == -1) {
            faults++;
            int victim = 0;
            
            int empty = -1;
            for(int j = 0; j < frames; j++) {
                if(mem[j] == -1) {
                    empty = j;
                    break;
                }
            }
            
            if(empty != -1) {
                mem[empty] = page_ref[i];
            } else {
                int farthest = -1;
                for(int j = 0; j < frames; j++) {
                    int next_use = 9999;
                    for(int k = i + 1; k < n; k++) {
                        if(page_ref[k] == mem[j]) {
                            next_use = k;
                            break;
                        }
                    }
                    if(next_use > farthest) {
                        farthest = next_use;
                        victim = j;
                    }
                }
                mem[victim] = page_ref[i];
            }
        }
        
        for(int j = 0; j < frames; j++) {
            if(mem[j] == -1) cout << "- ";
            else cout << mem[j] << " ";
        }
        cout << (pos == -1 ? " FAULT" : " HIT") << endl;
    }
    cout << "Total Faults: " << faults << endl;
}

void lru(int frames) {
    int mem[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int time[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int faults = 0;
    
    cout << "\n=== LRU ===" << endl;
    cout << "Ref  Frames" << endl;
    
    for(int i = 0; i < n; i++) {
        int pos = -1;
        for(int j = 0; j < frames; j++) {
            if(mem[j] == page_ref[i]) {
                pos = j;
                break;
            }
        }
        
        cout << page_ref[i] << "    ";
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
            mem[lru_idx] = page_ref[i];
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
    int frames;
    
    cout << "Enter number of references: ";
    cin >> n;
    cout << "Enter number of frames: ";
    cin >> frames;
    
    cout << "Enter page reference string:\n";
    for(int i = 0; i < n; i++) cin >> page_ref[i];
    
    cout << "\n1. OPTIMAL\n2. LRU\nChoice: ";
    int choice;
    cin >> choice;
    
    if(choice == 1) optimal(frames);
    else if(choice == 2) lru(frames);
    
    return 0;
}