

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

#define MAX_FRAMES 10
#define MAX_PAGES 50

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

int fifo(int pages[], int n, int frameSize) {
    int frames[MAX_FRAMES];
    int pageFaults = 0;
    int nextReplaceIndex = 0;
    

    for(int i = 0; i < frameSize; i++)
        frames[i] = -1;
    
    cout << "\n--- FIFO Page Replacement ---" << endl;
    cout << "Reference  Frames           Fault/Hit" << endl;
    cout << "--------------------------------------" << endl;
    
    for(int i = 0; i < n; i++) {
        int page = pages[i];
        cout << "   " << page << "       ";
        

        if(findPage(frames, frameSize, page) != -1) {

            displayFrames(frames, frameSize);
            cout << "  HIT" << endl;
        }
        else {

            pageFaults++;
            

            frames[nextReplaceIndex] = page;
            nextReplaceIndex = (nextReplaceIndex + 1) % frameSize;
            
            displayFrames(frames, frameSize);
            cout << "  FAULT" << endl;
        }
    }
    
    return pageFaults;
}

int lru(int pages[], int n, int frameSize) {
    int frames[MAX_FRAMES];
    int lastUsed[MAX_FRAMES];
    int pageFaults = 0;
    

    for(int i = 0; i < frameSize; i++) {
        frames[i] = -1;
        lastUsed[i] = -1;
    }
    
    cout << "\n--- LRU Page Replacement ---" << endl;
    cout << "Reference  Frames           Fault/Hit" << endl;
    cout << "--------------------------------------" << endl;
    
    for(int i = 0; i < n; i++) {
        int page = pages[i];
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
            

            frames[victimIndex] = page;
            lastUsed[victimIndex] = i;
            
            displayFrames(frames, frameSize);
            cout << "  FAULT" << endl;
        }
    }
    
    return pageFaults;
}

int main() {
    srand(time(0));
    
    cout << "========================================" << endl;
    cout << "  PAGE REPLACEMENT - FIFO & LRU" << endl;
    cout << "========================================\n" << endl;
    
    int n, frameSize;
    int pages[MAX_PAGES];
    

    cout << "Enter number of page references: ";
    cin >> n;
    
    cout << "Enter frame size (1-7): ";
    cin >> frameSize;
    
    if(frameSize < 1 || frameSize > 7) {
        cout << "Invalid frame size! Must be 1-7" << endl;
        return 1;
    }
    
    cout << "\n1. Enter page reference string manually" << endl;
    cout << "2. Generate random page reference string (0-9)" << endl;
    cout << "Choice: ";
    int choice;
    cin >> choice;
    
    if(choice == 1) {
        cout << "\nEnter page reference string:" << endl;
        for(int i = 0; i < n; i++) {
            cin >> pages[i];
        }
    }
    else {

        cout << "\nGenerating random page reference string..." << endl;
        for(int i = 0; i < n; i++) {
            pages[i] = rand() % 10;
        }
    }
    

    cout << "\nPage Reference String: ";
    for(int i = 0; i < n; i++) {
        cout << pages[i] << " ";
    }
    cout << endl;
    cout << "Frame Size: " << frameSize << endl;
    

    cout << "\n========================================" << endl;
    int fifoFaults = fifo(pages, n, frameSize);
    cout << "\nTotal Page Faults (FIFO): " << fifoFaults << endl;
    cout << "Page Fault Rate (FIFO): " << (fifoFaults * 100.0 / n) << "%" << endl;
    

    cout << "\n========================================" << endl;
    int lruFaults = lru(pages, n, frameSize);
    cout << "\nTotal Page Faults (LRU): " << lruFaults << endl;
    cout << "Page Fault Rate (LRU): " << (lruFaults * 100.0 / n) << "%" << endl;
    

    cout << "\n========================================" << endl;
    cout << "  COMPARISON" << endl;
    cout << "========================================" << endl;
    cout << "FIFO Page Faults: " << fifoFaults << endl;
    cout << "LRU Page Faults:  " << lruFaults << endl;
    
    if(fifoFaults < lruFaults)
        cout << "\nFIFO performed better!" << endl;
    else if(lruFaults < fifoFaults)
        cout << "\nLRU performed better!" << endl;
    else
        cout << "\nBoth performed equally!" << endl;
    
    cout << "\n========================================" << endl;
    
    return 0;
}

