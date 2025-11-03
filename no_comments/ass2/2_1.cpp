

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

#define MAX 100

void bubbleSort(int arr[], int n) {
    cout << "\n[PARENT - PID: " << getpid() << "] Starting Bubble Sort..." << endl;
    
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(arr[j] > arr[j+1]) {

                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
    
    cout << "[PARENT] Bubble Sort Result: ";
    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void insertionSort(int arr[], int n) {
    cout << "\n[CHILD - PID: " << getpid() << ", Parent PID: " << getppid() 
         << "] Starting Insertion Sort..." << endl;
    
    for(int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        

        while(j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
    
    cout << "[CHILD] Insertion Sort Result: ";
    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void demonstrateZombie(int arr[], int n) {
    cout << "\n========================================" << endl;
    cout << "  DEMONSTRATING ZOMBIE STATE" << endl;
    cout << "========================================" << endl;
    
    pid_t pid = fork();
    
    if(pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if(pid == 0) {

        insertionSort(arr, n);
        cout << "[CHILD] Exiting now..." << endl;
        exit(0);
    }
    else {

        cout << "[PARENT] Child PID: " << pid << endl;
        cout << "[PARENT] Sleeping for 5 seconds (child becomes ZOMBIE)..." << endl;
        sleep(5);
        
        cout << "[PARENT] Now calling wait() to reap zombie child..." << endl;
        wait(NULL);
        cout << "[PARENT] Zombie child reaped successfully!" << endl;
        
        bubbleSort(arr, n);
    }
}

void demonstrateOrphan(int arr[], int n) {
    cout << "\n========================================" << endl;
    cout << "  DEMONSTRATING ORPHAN STATE" << endl;
    cout << "========================================" << endl;
    
    pid_t pid = fork();
    
    if(pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if(pid == 0) {

        cout << "[CHILD] Initial Parent PID: " << getppid() << endl;
        sleep(3);
        cout << "[CHILD] After parent exit, new Parent PID: " << getppid() 
             << " (adopted by init)" << endl;
        insertionSort(arr, n);
        exit(0);
    }
    else {

        cout << "[PARENT] My PID: " << getpid() << ", Child PID: " << pid << endl;
        cout << "[PARENT] Exiting immediately (child becomes ORPHAN)..." << endl;
        exit(0);
    }
}

void normalExecution(int arr[], int n) {
    cout << "\n========================================" << endl;
    cout << "  NORMAL EXECUTION WITH WAIT()" << endl;
    cout << "========================================" << endl;
    
    pid_t pid = fork();
    
    if(pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if(pid == 0) {

        insertionSort(arr, n);
        exit(0);
    }
    else {

        cout << "[PARENT] Waiting for child to complete..." << endl;
        wait(NULL);
        cout << "[PARENT] Child completed. Now starting parent work..." << endl;
        bubbleSort(arr, n);
    }
}

int main() {
    int arr[MAX], n, choice;
    
    cout << "========================================" << endl;
    cout << "  FORK WITH SORTING" << endl;
    cout << "  (Zombie & Orphan Demonstration)" << endl;
    cout << "========================================" << endl;
    

    cout << "\nEnter number of elements (max " << MAX << "): ";
    cin >> n;
    
    if(n <= 0 || n > MAX) {
        cout << "Invalid input!" << endl;
        return 1;
    }
    
    cout << "Enter " << n << " integers: ";
    for(int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    

    cout << "\n========================================" << endl;
    cout << "           MENU" << endl;
    cout << "========================================" << endl;
    cout << "1. Normal Execution (with wait())" << endl;
    cout << "2. Demonstrate Zombie State" << endl;
    cout << "3. Demonstrate Orphan State" << endl;
    cout << "========================================" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    

    int arrCopy[MAX];
    for(int i = 0; i < n; i++) {
        arrCopy[i] = arr[i];
    }
    
    switch(choice) {
        case 1:
            normalExecution(arrCopy, n);
            break;
        case 2:
            demonstrateZombie(arrCopy, n);
            break;
        case 3:
            demonstrateOrphan(arrCopy, n);
            cout << "\nNote: Parent exited. If you see orphan child output, ";
            cout << "it was adopted by init process." << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
    }
    
    return 0;
}

