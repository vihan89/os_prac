#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int cmp(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void disk_schedule(int req[], int n, int head, int size, bool cscan) {
    int left[50], right[50], lc = 0, rc = 0;
    
    for(int i = 0; i < n; i++)
        req[i] < head ? (left[lc++] = req[i]) : (right[rc++] = req[i]);
    
    qsort(left, lc, sizeof(int), cmp);
    qsort(right, rc, sizeof(int), cmp);
    
    printf("\nSequence: %d", head);
    int total = 0, prev = head;
    
    for(int i = 0; i < rc; i++) {
        printf(" -> %d", right[i]);
        total += right[i] - prev;
        prev = right[i];
    }
    
    if(cscan && rc > 0 && prev != size-1) {
        printf(" -> %d", size-1);
        total += size-1 - prev;
        prev = size-1;
    }
    
    if(lc > 0) {
        int jump = cscan ? 0 : left[0];
        printf(" -> %d", jump);
        total += abs(prev - jump);
        prev = jump;
        
        for(int i = (cscan ? 0 : 1); i < lc; i++) {
            printf(" -> %d", left[i]);
            total += left[i] - prev;
            prev = left[i];
        }
    }
    
    printf("\nTotal: %d\n", total);
}

int main() {
    int n, head, size, req[50];
    printf("Disk size: "); scanf("%d", &size);
    printf("Head position: "); scanf("%d", &head);
    printf("Requests: "); scanf("%d", &n);
    for(int i = 0; i < n; i++) scanf("%d", &req[i]);
    printf("1.C-SCAN 2.C-LOOK: ");
    int ch; scanf("%d", &ch);
    disk_schedule(req, n, head, size, ch == 1);
    return 0;
}
