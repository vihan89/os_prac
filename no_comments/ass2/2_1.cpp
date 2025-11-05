#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

void bubbleSort(int arr[], int n) {
    for(int i = 0; i < n-1; i++)
        for(int j = 0; j < n-i-1; j++)
            if(arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
    cout << "Parent sorted: ";
    for(int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << endl;
}

void insertionSort(int arr[], int n) {
    for(int i = 1; i < n; i++) {
        int key = arr[i], j = i - 1;
        while(j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
    cout << "Child sorted: ";
    for(int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << endl;
}

int main() {
    int n, arr[100];
    
    cout << "Enter number of elements: ";
    cin >> n;
    cout << "Enter elements:\n";
    for(int i = 0; i < n; i++) cin >> arr[i];
    
    cout << "\n1. Normal fork\n2. Zombie demo\n3. Orphan demo\nChoice: ";
    int choice;
    cin >> choice;
    
    pid_t pid = fork();
    
    if(pid == 0) {
        if(choice == 3) {
            cout << "Child: parent was " << getppid();
            sleep(3);
            cout << ", now " << getppid() << " (orphan!)" << endl;
        }
        insertionSort(arr, n);
    } else {
        if(choice == 2) {
            cout << "Parent sleeping (child becomes zombie)...\n";
            sleep(5);
        }
        if(choice != 3) wait(NULL);
        bubbleSort(arr, n);
    }
    
    return 0;
}

