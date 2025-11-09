#include <stdio.h>
#include <stdbool.h>

int n, m;
int avail[10], max_m[10][10], alloc[10][10], need[10][10];

void calcNeed() {
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            need[i][j] = max_m[i][j] - alloc[i][j];
}

bool isSafe(int safe[]) {
    int work[10], finish[10] = {0}, count = 0;
    for(int i = 0; i < m; i++) work[i] = avail[i];
    
    while(count < n) {
        bool found = false;
        for(int i = 0; i < n; i++) {
            if(!finish[i]) {
                bool can = true;
                for(int j = 0; j < m; j++)
                    if(need[i][j] > work[j]) { can = false; break; }
                if(can) {
                    for(int j = 0; j < m; j++) work[j] += alloc[i][j];
                    safe[count++] = i;
                    finish[i] = 1;
                    found = true;
                }
            }
        }
        if(!found) return false;
    }
    return true;
}

int main() {
    printf("Enter processes, resources: ");
    scanf("%d %d", &n, &m);
    
    printf("Enter Available:\n");
    for(int i = 0; i < m; i++) scanf("%d", &avail[i]);
    
    printf("Enter Max:\n");
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++) scanf("%d", &max_m[i][j]);
    
    printf("Enter Allocation:\n");
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++) scanf("%d", &alloc[i][j]);
    
    calcNeed();
    
    int p, req[10], safe[10];
    printf("Enter process requesting: ");
    scanf("%d", &p);
    printf("Enter request:\n");
    for(int i = 0; i < m; i++) scanf("%d", &req[i]);
    
    bool valid = true;
    for(int i = 0; i < m; i++)
        if(req[i] > need[p][i] || req[i] > avail[i]) { valid = false; break; }
    
    if(!valid) {
        printf("Request invalid or unavailable\n");
        return 0;
    }
    
    for(int i = 0; i < m; i++) {
        avail[i] -= req[i];
        alloc[p][i] += req[i];
        need[p][i] -= req[i];
    }
    
    if(isSafe(safe)) {
        printf("SAFE. Granted. Sequence: ");
        for(int i = 0; i < n; i++) printf("P%d ", safe[i]);
        printf("\n");
    } else {
        printf("UNSAFE. Denied.\n");
        for(int i = 0; i < m; i++) {
            avail[i] += req[i];
            alloc[p][i] -= req[i];
            need[p][i] += req[i];
        }
    }
    
    return 0;
}
