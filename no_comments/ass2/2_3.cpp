#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cmath>
using namespace std;

bool isPrime(int n) {
    if(n < 2) return false;
    if(n == 2) return true;
    if(n % 2 == 0) return false;
    for(int i = 3; i <= sqrt(n); i += 2)
        if(n % i == 0) return false;
    return true;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "Usage: " << argv[0] << " <count>" << endl;
        return 1;
    }
    
    int n = atoi(argv[1]);
    if(n <= 0) {
        cout << "Count must be positive!" << endl;
        return 1;
    }
    
    pid_t pid = fork();
    
    if(pid == 0) {
        cout << "Child generating " << n << " primes:\n";
        int count = 0, num = 2;
        while(count < n) {
            if(isPrime(num)) {
                cout << num << " ";
                count++;
            }
            num++;
        }
        cout << endl;
    } else {
        wait(NULL);
        cout << "Parent: Child finished generating primes." << endl;
    }
    
    return 0;
}

