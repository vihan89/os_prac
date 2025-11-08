#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

bool isPrime(int n) {
    if(n < 2) return false;
    for(int i = 2; i * i <= n; i++) {
        if(n % i == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "Usage: ./program <number_of_primes>" << endl;
        return 1;
    }
    
    int n = atoi(argv[1]);
    
    if(n <= 0) {
        cout << "Error: Please provide a positive integer" << endl;
        return 1;
    }
    
    pid_t pid = fork();
    
    if(pid == 0) {
        cout << "Child Process: Generating " << n << " prime numbers" << endl;
        int count = 0;
        int num = 2;
        
        while(count < n) {
            if(isPrime(num)) {
                cout << num << " ";
                count++;
            }
            num++;
        }
        cout << endl;
    } 
    else {
        wait(NULL);
        cout << "Parent Process: Child has finished execution" << endl;
    }
    
    return 0;
}

