/*
==============================================================================
ASSIGNMENT 2.3: PRIME NUMBERS GENERATION WITH FORK
==============================================================================

PROBLEM:
- Accept number N from command line argument
- Child process generates first N prime numbers
- Parent waits for child completion
- Use simple prime checking algorithm

COMPILATION:
g++ 2_3.cpp -o 2_3

EXECUTION:
./2_3 10        (generates first 10 prime numbers)
./2_3 20        (generates first 20 prime numbers)

KEY CONCEPTS TO REMEMBER:
1. Command line arguments: argc, argv[]
2. fork() creates child process
3. wait() makes parent wait for child
4. Prime number: divisible only by 1 and itself
5. Optimization: check divisors only up to sqrt(n)

SAMPLE RUN:
./2_3 10
First 10 prime numbers: 2 3 5 7 11 13 17 19 23 29
==============================================================================
*/

#include <iostream>
#include <unistd.h>     // fork(), getpid(), getppid()
#include <sys/wait.h>   // wait()
#include <cstdlib>      // exit(), atoi()
#include <cmath>        // sqrt()

using namespace std;

// ============================================================================
// CHECK IF NUMBER IS PRIME
// Time Complexity: O(√n)
// ============================================================================
bool isPrime(int num) {
    // Numbers less than 2 are not prime
    if(num < 2) {
        return false;
    }
    
    // 2 is prime
    if(num == 2) {
        return true;
    }
    
    // Even numbers (except 2) are not prime
    if(num % 2 == 0) {
        return false;
    }
    
    // Check odd divisors from 3 to √num
    // Optimization: No need to check beyond √num
    // If n = a × b and a ≤ b, then a ≤ √n
    int limit = sqrt(num);
    for(int i = 3; i <= limit; i += 2) {
        if(num % i == 0) {
            return false;  // Found a divisor, not prime
        }
    }
    
    return true;  // No divisors found, is prime
}

// ============================================================================
// GENERATE FIRST N PRIME NUMBERS
// Child process function
// ============================================================================
void generatePrimes(int n) {
    int count = 0;      // Count of primes found
    int num = 2;        // Start checking from 2
    int primes[1000];   // Store prime numbers
    
    cout << "\n[CHILD - PID: " << getpid() << ", Parent PID: " << getppid() << "]" << endl;
    cout << "[CHILD] Generating first " << n << " prime numbers..." << endl;
    
    // Find first n prime numbers
    while(count < n) {
        if(isPrime(num)) {
            primes[count] = num;
            count++;
        }
        num++;
    }
    
    // Display the prime numbers
    cout << "\n========================================" << endl;
    cout << "  FIRST " << n << " PRIME NUMBERS" << endl;
    cout << "========================================" << endl;
    
    for(int i = 0; i < n; i++) {
        cout << primes[i];
        if(i < n - 1) {
            cout << ", ";
        }
        // Print newline after every 10 numbers for readability
        if((i + 1) % 10 == 0 && i < n - 1) {
            cout << "\n";
        }
    }
    cout << "\n========================================" << endl;
    
    cout << "\n[CHILD] Prime generation completed!" << endl;
    cout << "[CHILD] Last prime generated: " << primes[n-1] << endl;
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main(int argc, char* argv[]) {
    cout << "========================================" << endl;
    cout << "  PRIME NUMBER GENERATOR" << endl;
    cout << "  (Using Fork)" << endl;
    cout << "========================================" << endl;
    
    // ========================================================================
    // VALIDATE COMMAND LINE ARGUMENTS
    // ========================================================================
    
    if(argc != 2) {
        cout << "\nError: Invalid number of arguments!" << endl;
        cout << "Usage: " << argv[0] << " <N>" << endl;
        cout << "  where N = number of prime numbers to generate" << endl;
        cout << "\nExample:" << endl;
        cout << "  " << argv[0] << " 10    (generates first 10 primes)" << endl;
        cout << "  " << argv[0] << " 25    (generates first 25 primes)" << endl;
        return 1;
    }
    
    // Convert argument to integer
    int n = atoi(argv[1]);
    
    // Validate input
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
    
    // ========================================================================
    // FORK TO CREATE CHILD PROCESS
    // ========================================================================
    
    pid_t pid = fork();
    
    if(pid < 0) {
        // Fork failed
        perror("[PARENT] Fork failed");
        return 1;
    }
    else if(pid == 0) {
        // ====================================================================
        // CHILD PROCESS - GENERATE PRIME NUMBERS
        // ====================================================================
        
        generatePrimes(n);
        
        exit(0);  // Child exits after generating primes
    }
    else {
        // ====================================================================
        // PARENT PROCESS - WAIT FOR CHILD
        // ====================================================================
        
        cout << "[PARENT] Child process created with PID: " << pid << endl;
        cout << "[PARENT] Waiting for child to complete..." << endl;
        
        int status;
        pid_t child_pid = wait(&status);  // Wait for child to finish
        
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

/*
==============================================================================
IMPORTANT NOTES FOR EXAM:

1. PRIME NUMBER DEFINITION:
   - A prime number is divisible only by 1 and itself
   - First few primes: 2, 3, 5, 7, 11, 13, 17, 19, 23, 29...
   - 1 is NOT a prime number
   - 2 is the only even prime number

2. PRIME CHECKING ALGORITHM:
   - Check if n < 2: not prime
   - Check if n == 2: prime
   - Check if n is even: not prime
   - Check odd divisors from 3 to √n
   - If no divisor found: prime

3. OPTIMIZATION - WHY CHECK ONLY UP TO √n:
   - If n = a × b where a ≤ b
   - Then a ≤ √n
   - If we don't find divisor ≤ √n, won't find any divisor
   - Example: For n=100, check only up to 10, not 99

4. FORK PROCESS CONCEPTS:
   - fork() creates exact copy of parent process
   - Returns 0 to child, child's PID to parent
   - Child has separate memory space
   - Both processes continue from fork() point

5. WAIT SYSTEM CALL:
   - Syntax: wait(&status)
   - Makes parent wait for any child to terminate
   - Returns PID of terminated child
   - status contains exit information
   - WIFEXITED(status): true if child exited normally
   - WEXITSTATUS(status): returns exit code

6. COMMAND LINE ARGUMENTS:
   - argc: argument count (including program name)
   - argv[0]: program name
   - argv[1] to argv[argc-1]: actual arguments
   - All arguments are strings (char*)
   - Use atoi() to convert to integer

7. TIME COMPLEXITY:
   - Checking if n is prime: O(√n)
   - Generating first N primes: O(N × √P)
     where P is the Nth prime number

8. EXAMPLE PRIME NUMBERS:
   - First 10: 2, 3, 5, 7, 11, 13, 17, 19, 23, 29
   - First 20: add 31, 37, 41, 43, 47, 53, 59, 61, 67, 71
   - 100th prime: 541
   - 1000th prime: 7919
==============================================================================
*/
