

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cmath>

using namespace std;

bool isPrime(int num) {

    if(num < 2) {
        return false;
    }
    

    if(num == 2) {
        return true;
    }
    

    if(num % 2 == 0) {
        return false;
    }
    

    int limit = sqrt(num);
    for(int i = 3; i <= limit; i += 2) {
        if(num % i == 0) {
            return false;
        }
    }
    
    return true;
}

void generatePrimes(int n) {
    int count = 0;
    int num = 2;
    int primes[1000];
    
    cout << "\n[CHILD - PID: " << getpid() << ", Parent PID: " << getppid() << "]" << endl;
    cout << "[CHILD] Generating first " << n << " prime numbers..." << endl;
    

    while(count < n) {
        if(isPrime(num)) {
            primes[count] = num;
            count++;
        }
        num++;
    }
    

    cout << "\n========================================" << endl;
    cout << "  FIRST " << n << " PRIME NUMBERS" << endl;
    cout << "========================================" << endl;
    
    for(int i = 0; i < n; i++) {
        cout << primes[i];
        if(i < n - 1) {
            cout << ", ";
        }

        if((i + 1) % 10 == 0 && i < n - 1) {
            cout << "\n";
        }
    }
    cout << "\n========================================" << endl;
    
    cout << "\n[CHILD] Prime generation completed!" << endl;
    cout << "[CHILD] Last prime generated: " << primes[n-1] << endl;
}

int main(int argc, char* argv[]) {
    cout << "========================================" << endl;
    cout << "  PRIME NUMBER GENERATOR" << endl;
    cout << "  (Using Fork)" << endl;
    cout << "========================================" << endl;
    

    
    if(argc != 2) {
        cout << "\nError: Invalid number of arguments!" << endl;
        cout << "Usage: " << argv[0] << " <N>" << endl;
        cout << "  where N = number of prime numbers to generate" << endl;
        cout << "\nExample:" << endl;
        cout << "  " << argv[0] << " 10    (generates first 10 primes)" << endl;
        cout << "  " << argv[0] << " 25    (generates first 25 primes)" << endl;
        return 1;
    }
    

    int n = atoi(argv[1]);
    

    if(n <= 0) {
        cout << "\nError: N must be a positive integer!" << endl;
        return 1;
    }
    
    if(n > 1000) {
        cout << "\nWarning: Generating " << n << " primes may take time." << endl;
        cout << "Consider using a smaller value (recommended: N <= 100)" << endl;
    }
    
    cout << "\n[PARENT - PID: " << getpid() << "] Starting program..." << endl;
    cout << "[PARENT] Request: Generate first " << n << " prime numbers" << endl;
    

    
    pid_t pid = fork();
    
    if(pid < 0) {

        perror("[PARENT] Fork failed");
        return 1;
    }
    else if(pid == 0) {

        
        generatePrimes(n);
        
        exit(0);
    }
    else {

        
        cout << "[PARENT] Child process created with PID: " << pid << endl;
        cout << "[PARENT] Waiting for child to complete..." << endl;
        
        int status;
        pid_t child_pid = wait(&status);
        
        if(WIFEXITED(status)) {
            cout << "\n[PARENT] Child (PID: " << child_pid << ") completed successfully" << endl;
            cout << "[PARENT] Child exit status: " << WEXITSTATUS(status) << endl;
        }
        else {
            cout << "\n[PARENT] Child terminated abnormally" << endl;
        }
        
        cout << "\n[PARENT] Program finished!" << endl;
    }
    
    return 0;
}

