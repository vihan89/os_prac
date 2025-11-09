# Operating Systems Lab - Complete Implementation Summary

## 📚 All Assignments Complete!

This workspace contains complete implementations of all 8 Operating Systems lab assignments with **22 total programs**.

---

## 📁 Directory Structure

```
os_prac/
├── ass2/          # Assignment 2 - Process Management (4 files)
├── ass3/          # Assignment 3 - CPU Scheduling (3 files)
├── ass4/          # Assignment 4 - Synchronization (4 files)
├── ass5/          # Assignment 5 - Banker's Algorithm (2 files)
├── ass6/          # Assignment 6 - Page Replacement (2 files)
├── ass7/          # Assignment 7 - IPC (4 files)
├── ass8/          # Assignment 8 - Disk Scheduling (2 files)
└── README.md      # This file
```

---

## ✅ Assignment 2: Process Management (4 files)

### 2_1.cpp - Fork with Sorting
- **Topics**: fork(), wait(), zombie, orphan processes
- **Features**: Bubble sort, insertion sort, 3 modes (normal/zombie/orphan)
- **Compile**: `g++ 2_1.cpp -o 2_1`

### 2_2.cpp - Parent-Child with execve
- **Topics**: execve(), command line arguments
- **Features**: Parent sorts array, child receives via execve
- **Compile**: `g++ 2_2.cpp -o 2_2`

### 2_2_search.cpp - Binary Search (called by 2_2)
- **Topics**: Binary search, argc/argv processing
- **Features**: Receives sorted array from parent
- **Compile**: `g++ 2_2_search.cpp -o 2_2_search`

### 2_3.cpp - Prime Number Generation
- **Topics**: fork(), prime checking
- **Features**: Child generates N primes using √n optimization
- **Compile**: `g++ 2_3.cpp -o 2_3`

---

## ✅ Assignment 3: CPU Scheduling (3 files)

### 3_1.cpp - FCFS & SJF Non-Preemptive
- **Topics**: First Come First Serve, Shortest Job First
- **Features**: TAT/WT calculation, Gantt chart, menu-driven
- **Compile**: `g++ 3_1.cpp -o 3_1`
- **Formulas**: TAT = CT - AT, WT = TAT - BT

### 3_2.cpp - FCFS & SRTF
- **Topics**: Shortest Remaining Time First (preemptive SJF)
- **Features**: Context switching, time-based scheduling
- **Compile**: `g++ 3_2.cpp -o 3_2`

### 3_3.cpp - FCFS & Round Robin
- **Topics**: Time quantum scheduling, queue implementation
- **Features**: TQ=2, custom Queue structure
- **Compile**: `g++ 3_3.cpp -o 3_3`

---

## ✅ Assignment 4: Synchronization (4 files)

### 4_1.cpp - Producer-Consumer (Simpson & Joey)
- **Topics**: Semaphores, mutex, bounded buffer
- **Features**: Simpson bakes pizzas, Joey eats, buffer size 5
- **Compile**: `g++ 4_1.cpp -o 4_1 -lpthread`
- **Use**: sem_t empty/full, pthread_mutex_t

### 4_2.cpp - Sleeping Teaching Assistant
- **Topics**: Semaphores, waiting chairs, student-TA interaction
- **Features**: 3 chairs, students wake TA
- **Compile**: `g++ 4_2.cpp -o 4_2 -lpthread`

### 4_3.cpp - Producer-Consumer with File I/O
- **Topics**: File-based synchronization, ofstream/ifstream
- **Features**: Producer writes to file, consumer reads
- **Compile**: `g++ 4_3.cpp -o 4_3 -lpthread`

### 4_4.cpp - Readers-Writers Problem
- **Topics**: Multiple readers, exclusive writer, reader priority
- **Features**: First reader locks, last reader unlocks
- **Compile**: `g++ 4_4.cpp -o 4_4 -lpthread`

---

## ✅ Assignment 5: Banker's Algorithm (2 files)

### 5_1.cpp - Basic Safety Check
- **Topics**: Deadlock avoidance, need matrix, safety algorithm
- **Features**: Need = Max - Allocation, safe sequence generation
- **Compile**: `g++ 5_1.cpp -o 5_1`
- **Formula**: Need[i][j] = Max[i][j] - Allocation[i][j]

### 5_2.cpp - With Resource Requests
- **Topics**: Resource request handling, pretend allocation, rollback
- **Features**: 4-step validation, grant/deny decisions
- **Compile**: `g++ 5_2.cpp -o 5_2`

---

## ✅ Assignment 6: Page Replacement (2 files)

### 6_1.cpp - FIFO & LRU
- **Topics**: Page faults, FIFO queue, LRU counter method
- **Features**: Random page string (0-9), variable frames (1-7)
- **Compile**: `g++ 6_1.cpp -o 6_1`
- **Formulas**: Fault Rate = (Faults / Total References) × 100%

### 6_2.cpp - Optimal & LRU
- **Topics**: Belady's optimal algorithm, look-ahead
- **Features**: Fixed string [1,2,3,4,1,2,5,1,1,2,3,4,5], frames 3 & 4
- **Compile**: `g++ 6_2.cpp -o 6_2`

---

## ✅ Assignment 7: IPC (4 files)

### 7_1_sender.cpp - Message Queue Sender
- **Topics**: System V IPC, msgget(), msgsnd()
- **Features**: ftok() key generation, message types
- **Compile**: `g++ 7_1_sender.cpp -o sender`

### 7_1_receiver.cpp - Message Queue Receiver
- **Topics**: msgrcv(), msgctl(), queue removal
- **Features**: Receives messages, removes queue on exit
- **Compile**: `g++ 7_1_receiver.cpp -o receiver`

### 7_2_server.cpp - Shared Memory Server
- **Topics**: shmget(), shmat(), shared memory segment
- **Features**: Writes data to shared memory
- **Compile**: `g++ 7_2_server.cpp -o server`

### 7_2_client.cpp - Shared Memory Client
- **Topics**: shmdt(), shmctl(), memory cleanup
- **Features**: Reads from shared memory, removes segment
- **Compile**: `g++ 7_2_client.cpp -o client`

---

## ✅ Assignment 8: Disk Scheduling (2 files)

### 8_1.cpp - C-SCAN & C-LOOK
- **Topics**: Circular SCAN, Circular LOOK, head movement
- **Features**: Disk 0-499, head=85, total seek time calculation
- **Compile**: `g++ 8_1.cpp -o 8_1`

### 8_2.cpp - SCAN & LOOK
- **Topics**: Elevator algorithm, bidirectional scanning
- **Features**: Disk 0-499, head=185, direction choice
- **Compile**: `g++ 8_2.cpp -o 8_2`

---

## 🎯 Key Features

### Code Quality
✅ **Arrays Only**: No vectors, uses `#define MAX` for size limits  
✅ **C++ with C System Calls**: fork(), exec, pthread, etc.  
✅ **Detailed Comments**: Every step explained  
✅ **Exam-Friendly**: Simple, clear, easy to understand  

### Each Program Includes
📝 Complete problem statement  
🔧 Compilation command  
▶️ Execution instructions  
📊 Sample input/output  
📐 Key formulas and concepts  
📚 Extensive exam notes (10-20 points per program)  
💡 Common mistakes and debugging tips  

### Exam Notes Cover
- Algorithm pseudocode
- Step-by-step calculations
- Time/Space complexity
- Advantages/Disadvantages
- Comparison with alternatives
- Real-world applications
- Common exam questions

---

## 🛠️ Compilation Guide

### Programs without threads:
```bash
g++ filename.cpp -o output
```

### Programs with threads (assignments 4):
```bash
g++ filename.cpp -o output -lpthread
```

---

## 📖 Topics Covered

### Process Management
- fork(), wait(), waitpid()
- execve() with arguments
- Zombie and orphan processes
- Prime number generation

### CPU Scheduling
- FCFS, SJF, SRTF, Round Robin
- Turnaround Time (TAT)
- Waiting Time (WT)
- Context switching

### Synchronization
- Semaphores (sem_t)
- Mutex (pthread_mutex_t)
- Producer-Consumer
- Readers-Writers
- File-based synchronization

### Deadlock Avoidance
- Banker's Algorithm
- Safety algorithm
- Need matrix calculation
- Resource request handling

### Page Replacement
- FIFO (First-In-First-Out)
- LRU (Least Recently Used)
- Optimal (Belady's)
- Page fault calculation

### IPC (Inter-Process Communication)
- Message Queues (msgsnd/msgrcv)
- Shared Memory (shmat/shmdt)
- ftok() key generation
- System V IPC

### Disk Scheduling
- SCAN (Elevator)
- LOOK
- C-SCAN (Circular SCAN)
- C-LOOK (Circular LOOK)
- Seek time calculation

---

## 📊 Quick Reference

### Important Formulas

**CPU Scheduling:**
```
Completion Time (CT) = When process finishes
Turnaround Time (TAT) = CT - Arrival Time
Waiting Time (WT) = TAT - Burst Time
Average TAT = Σ(TAT) / n
Average WT = Σ(WT) / n
```

**Banker's Algorithm:**
```
Need[i][j] = Max[i][j] - Allocation[i][j]
Safe if: ∃ sequence where Need[i] ≤ Available for all i
```

**Page Replacement:**
```
Page Fault Rate = (Page Faults / Total References) × 100%
```

**Disk Scheduling:**
```
Total Seek Time = Σ|current - next|
Average Seek Time = Total Seek Time / (n-1)
```

---

## 🚀 Running Programs

### Assignment 2 Examples:
```bash
cd ass2
g++ 2_1.cpp -o 2_1
./2_1

g++ 2_2.cpp -o 2_2
g++ 2_2_search.cpp -o 2_2_search
./2_2

g++ 2_3.cpp -o 2_3
./2_3 10   # Generate 10 primes
```

### Assignment 4 Examples:
```bash
cd ass4
g++ 4_1.cpp -o 4_1 -lpthread
./4_1

g++ 4_4.cpp -o 4_4 -lpthread
./4_4
```

### Assignment 7 Examples:
```bash
cd ass7

# Terminal 1
g++ 7_1_sender.cpp -o sender
./sender

# Terminal 2
g++ 7_1_receiver.cpp -o receiver
./receiver
```

---

## 📝 Exam Preparation Tips

1. **Understand Concepts First**: Read the extensive exam notes in each file
2. **Trace Execution**: Follow step-by-step execution with sample data
3. **Memorize Formulas**: TAT, WT, Need matrix, page fault rate
4. **Practice Calculations**: Work through examples manually
5. **Know Trade-offs**: Advantages/disadvantages of each algorithm
6. **Compare Algorithms**: Know when to use which (FIFO vs LRU, SCAN vs C-SCAN)

---

## 🔍 Common Exam Questions

### Process Management
- Difference between zombie and orphan?
- When to use fork() vs execve()?
- What happens if wait() is not called?

### CPU Scheduling
- Calculate TAT and WT for given processes
- Which algorithm gives minimum average WT?
- What is convoy effect? (FCFS)
- Does Round Robin eliminate starvation?

### Synchronization
- How to prevent race condition?
- Producer-Consumer: Why need both semaphores and mutex?
- Readers-Writers: What is starvation problem?

### Banker's Algorithm
- Is this state safe? Find safe sequence
- Can this request be granted?
- What is difference between safe and unsafe state?

### Page Replacement
- Calculate page faults for FIFO/LRU/Optimal
- What is Belady's Anomaly?
- Which algorithm is theoretical best?

### Disk Scheduling
- Calculate total seek time for SCAN/C-SCAN
- Why is LOOK better than SCAN?
- What is elevator algorithm?

---

## 📊 Difficulty Levels & Learning Order

### **Recommended Learning Path (Easiest → Hardest):**

#### ⭐ **Level 1: EASIEST** - Start Here!
**Assignment 3 (CPU Scheduling)**
- Pure logic, no system calls
- Just loops and arrays
- Perfect for beginners
- **Start with:** `3_1.cpp` (FCFS & SJF)

#### ⭐⭐ **Level 2: VERY EASY**
**Assignment 6 (Page Replacement)**
- Arrays and counters
- No system calls
- Clear algorithm steps
- **Try:** `6_1.cpp` (FIFO & LRU)

**Assignment 8 (Disk Scheduling)**
- Array sorting
- Simple calculations
- No system calls
- **Try:** `8_1.cpp` (C-SCAN & C-LOOK)

#### ⭐⭐⭐ **Level 3: MODERATE**
**Assignment 5 (Banker's Algorithm)**
- 2D arrays
- Multiple matrices
- Logic is straightforward once you understand the concept
- **Start with:** `5_1.cpp` (Basic safety check)

#### ⭐⭐⭐⭐ **Level 4: HARDER** - New Concepts
**Assignment 2 (Process Management)**
- Introduces `fork()`, `exec()`
- System calls (new concept)
- Process behavior understanding needed
- **Start with:** `2_1.cpp` (Basic fork)

**Assignment 4 (Synchronization)**
- Threads (new concept)
- Semaphores, mutex
- Concurrent programming
- Race conditions to understand
- **Start with:** `4_1.cpp` (Producer-Consumer)

#### ⭐⭐⭐⭐⭐ **Level 5: HARDEST** - Most Complex
**Assignment 7 (IPC)**
- Message queues (complex API)
- Shared memory (pointer usage)
- Semaphore synchronization
- Multi-process coordination
- **Try Message Queue first:** `7_1_sender.cpp` & `7_1_receiver.cpp`

---

## 🎓 Learning Outcomes

After completing these assignments, you will understand:

✅ Process creation and management in Unix/Linux  
✅ CPU scheduling algorithms and their performance metrics  
✅ Thread synchronization and classic synchronization problems  
✅ Deadlock avoidance using Banker's Algorithm  
✅ Virtual memory and page replacement strategies  
✅ Inter-process communication mechanisms  
✅ Disk scheduling algorithms and seek time optimization  

---

## 📚 Additional Resources

- **System Calls**: man fork, man exec, man pthread_create
- **IPC Commands**: ipcs, ipcrm
- **Debugging**: gdb for debugging, valgrind for memory leaks

---

## ⚠️ Important Notes

1. **Compilation**: Use g++ for all programs
2. **Permissions**: Some programs need sudo for IPC operations
3. **Cleanup**: Remove IPC resources manually if programs crash
   - Message Queues: `ipcrm -q <msgid>`
   - Shared Memory: `ipcrm -m <shmid>`
4. **Thread Programs**: Always link with `-lpthread`

---

## 🏆 Complete Checklist

- [x] Assignment 2: Process Management (4 files)
- [x] Assignment 3: CPU Scheduling (3 files)
- [x] Assignment 4: Synchronization (4 files)
- [x] Assignment 5: Banker's Algorithm (2 files)
- [x] Assignment 6: Page Replacement (2 files)
- [x] Assignment 7: IPC (4 files)
- [x] Assignment 8: Disk Scheduling (2 files)

**Total: 22 complete, exam-ready programs! 🎉**

---

## 💻 Author Notes

All programs are:
- ✅ Tested and working
- ✅ Well-commented for understanding
- ✅ Exam-friendly (simple and clear)
- ✅ Include detailed explanations
- ✅ Ready to compile and run
- ✅ Follow coding best practices

Good luck with your exams! 📖✨
