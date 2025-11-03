

#include <iostream>
#include <climits>

using namespace std;

#define MAX_FRAMES 10

int refString[] = {1, 2, 3, 4, 1, 2, 5, 1, 1, 2, 3, 4, 5};
int refLength = 13;

int findPage(int frames[], int frameSize, int page) {
    for(int i = 0; i < frameSize; i++) {
        if(frames[i] == page)
            return i;
    }
    return -1;
}

void displayFrames(int frames[], int frameSize) {
    cout << "| ";
    for(int i = 0; i < frameSize; i++) {
        if(frames[i] == -1)
            cout << "- ";
        else
            cout << frames[i] << " ";
    }
    cout << "|";
}

int findNextUse(int currentPos) {

    return currentPos;
}

int findOptimalVictim(int frames[], int frameSize, int currentPos) {
    int victim = 0;
    int farthest = -1;
    
    for(int i = 0; i < frameSize; i++) {
        int j;

        for(j = currentPos; j < refLength; j++) {
            if(refString[j] == frames[i])
                break;
        }
        

        if(j > farthest) {
            farthest = j;
            victim = i;
        }
    }
    
    return victim;
}

int optimal(int frameSize) {
    int frames[MAX_FRAMES];
    int pageFaults = 0;
    

    for(int i = 0; i < frameSize; i++)
        frames[i] = -1;
    
    cout << "\n--- OPTIMAL Page Replacement ---" << endl;
    cout << "Reference  Frames           Fault/Hit  Explanation" << endl;
    cout << "--------------------------------------------------------" << endl;
    
    for(int i = 0; i < refLength; i++) {
        int page = refString[i];
        cout << "   " << page << "       ";
        
        int pageIndex = findPage(frames, frameSize, page);
        
        if(pageIndex != -1) {

            displayFrames(frames, frameSize);
            cout << "  HIT" << endl;
        }
        else {

            pageFaults++;
            

            int emptyIndex = findPage(frames, frameSize, -1);
            
            if(emptyIndex != -1) {

                frames[emptyIndex] = page;
                displayFrames(frames, frameSize);
                cout << "  FAULT   (Empty slot)" << endl;
            }
            else {

                int victimIndex = findOptimalVictim(frames, frameSize, i + 1);
                int victimPage = frames[victimIndex];
                frames[victimIndex] = page;
                
                displayFrames(frames, frameSize);
                cout << "  FAULT   (Replace " << victimPage << ")" << endl;
            }
        }
    }
    
    return pageFaults;
}

int lru(int frameSize) {
    int frames[MAX_FRAMES];
    int lastUsed[MAX_FRAMES];
    int pageFaults = 0;
    

    for(int i = 0; i < frameSize; i++) {
        frames[i] = -1;
        lastUsed[i] = -1;
    }
    
    cout << "\n--- LRU Page Replacement ---" << endl;
    cout << "Reference  Frames           Fault/Hit  Explanation" << endl;
    cout << "--------------------------------------------------------" << endl;
    
    for(int i = 0; i < refLength; i++) {
        int page = refString[i];
        cout << "   " << page << "       ";
        
        int pageIndex = findPage(frames, frameSize, page);
        
        if(pageIndex != -1) {

            lastUsed[pageIndex] = i;
            displayFrames(frames, frameSize);
            cout << "  HIT" << endl;
        }
        else {

            pageFaults++;
            

            int victimIndex = 0;
            int minTime = lastUsed[0];
            
            for(int j = 1; j < frameSize; j++) {
                if(lastUsed[j] < minTime) {
                    minTime = lastUsed[j];
                    victimIndex = j;
                }
            }
            
            int victimPage = frames[victimIndex];
            frames[victimIndex] = page;
            lastUsed[victimIndex] = i;
            
            displayFrames(frames, frameSize);
            if(victimPage == -1)
                cout << "  FAULT   (Empty slot)" << endl;
            else
                cout << "  FAULT   (Replace " << victimPage << ")" << endl;
        }
    }
    
    return pageFaults;
}

int main() {
    cout << "========================================" << endl;
    cout << "  PAGE REPLACEMENT - OPTIMAL & LRU" << endl;
    cout << "========================================\n" << endl;
    

    cout << "Fixed Page Reference String:" << endl;
    for(int i = 0; i < refLength; i++) {
        cout << refString[i] << " ";
    }
    cout << "\n(Total references: " << refLength << ")" << endl;
    

    cout << "\n========================================" << endl;
    cout << "  FRAME SIZE = 3" << endl;
    cout << "========================================" << endl;
    
    int optimalFaults3 = optimal(3);
    cout << "\nTotal Page Faults (Optimal): " << optimalFaults3 << endl;
    cout << "Page Fault Rate (Optimal): " << (optimalFaults3 * 100.0 / refLength) << "%" << endl;
    
    cout << endl;
    
    int lruFaults3 = lru(3);
    cout << "\nTotal Page Faults (LRU): " << lruFaults3 << endl;
    cout << "Page Fault Rate (LRU): " << (lruFaults3 * 100.0 / refLength) << "%" << endl;
    

    cout << "\n========================================" << endl;
    cout << "  FRAME SIZE = 4" << endl;
    cout << "========================================" << endl;
    
    int optimalFaults4 = optimal(4);
    cout << "\nTotal Page Faults (Optimal): " << optimalFaults4 << endl;
    cout << "Page Fault Rate (Optimal): " << (optimalFaults4 * 100.0 / refLength) << "%" << endl;
    
    cout << endl;
    
    int lruFaults4 = lru(4);
    cout << "\nTotal Page Faults (LRU): " << lruFaults4 << endl;
    cout << "Page Fault Rate (LRU): " << (lruFaults4 * 100.0 / refLength) << "%" << endl;
    

    cout << "\n========================================" << endl;
    cout << "  FINAL COMPARISON" << endl;
    cout << "========================================" << endl;
    
    cout << "\nFrame Size 3:" << endl;
    cout << "  Optimal: " << optimalFaults3 << " faults" << endl;
    cout << "  LRU:     " << lruFaults3 << " faults" << endl;
    cout << "  Difference: " << (lruFaults3 - optimalFaults3) << endl;
    
    cout << "\nFrame Size 4:" << endl;
    cout << "  Optimal: " << optimalFaults4 << " faults" << endl;
    cout << "  LRU:     " << lruFaults4 << " faults" << endl;
    cout << "  Difference: " << (lruFaults4 - optimalFaults4) << endl;
    
    cout << "\nObservation:" << endl;
    if(optimalFaults3 <= lruFaults3 && optimalFaults4 <= lruFaults4)
        cout << "Optimal always performs better or equal to LRU" << endl;
    
    cout << "\n========================================" << endl;
    
    return 0;
}

