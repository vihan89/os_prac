#include <stdio.h>
#include <stdbool.h>

struct Process {
    int pid, at, bt, rt, ct, tat, wt;
};

typedef struct {
    int data[100];
    int front, rear, size;
} Queue;

void initQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}

void enqueue(Queue *q, int val) {
    q->data[++q->rear] = val;
    q->size++;
}

int dequeue(Queue *q) {
    q->size--;
    return q->data[q->front++];
}

bool isEmpty(Queue *q) {
    return q->size == 0;
}

void roundRobin(struct Process p[], int n, int tq) {
    printf("\nRound Robin (Time Quantum = %d):\n", tq);
    for(int i = 0; i < n; i++) p[i].rt = p[i].bt;
    
    Queue q;
    initQueue(&q);
    bool inQ[100] = {false};
    int t = 0, done = 0;
    
    for(int i = 0; i < n; i++) {
        if(p[i].at <= t) {
            enqueue(&q, i);
            inQ[i] = true;
        }
    }
    
    while(done < n) {
        if(isEmpty(&q)) {
            t++;
            for(int i = 0; i < n; i++) {
                if(p[i].at <= t && !inQ[i] && p[i].rt > 0) {
                    enqueue(&q, i);
                    inQ[i] = true;
                }
            }
            continue;
        }
        
        int curr = dequeue(&q);
        
        int exec = (p[curr].rt < tq) ? p[curr].rt : tq;
        p[curr].rt -= exec;
        t += exec;
        
        for(int i = 0; i < n; i++) {
            if(p[i].at <= t && !inQ[i] && p[i].rt > 0) {
                enqueue(&q, i);
                inQ[i] = true;
            }
        }
        
        if(p[curr].rt == 0) {
            p[curr].ct = t;
            p[curr].tat = p[curr].ct - p[curr].at;
            p[curr].wt = p[curr].tat - p[curr].bt;
            done++;
        } else {
            enqueue(&q, curr);
        }
    }
    
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    float avgTAT = 0, avgWT = 0;
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, 
               p[i].ct, p[i].tat, p[i].wt);
        avgTAT += p[i].tat;
        avgWT += p[i].wt;
    }
    printf("Avg TAT: %.2f | Avg WT: %.2f\n", avgTAT/n, avgWT/n);
}

int main() {
    int n, tq;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter time quantum: ");
    scanf("%d", &tq);
    
    struct Process p[n];
    for(int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("P%d - Arrival Time: ", p[i].pid);
        scanf("%d", &p[i].at);
        printf("P%d - Burst Time: ", p[i].pid);
        scanf("%d", &p[i].bt);
    }
    
    roundRobin(p, n, tq);
    return 0;
}
