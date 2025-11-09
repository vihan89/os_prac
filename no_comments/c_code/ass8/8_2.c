#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int cmp(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void disk_schedule(int req[], int n, int head, int size, bool scan) {
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
    
    if(scan && (rc == 0 || prev != size-1)) {
        printf(" -> %d", size-1);
        total += size-1 - prev;
        prev = size-1;
    }
    
    for(int i = lc-1; i >= 0; i--) {
        printf(" -> %d", left[i]);
        total += prev - left[i];
        prev = left[i];
    }
    
    printf("\nTotal: %d\n", total);
}

int main() {
    int n, head, size, req[50];
    printf("Disk size: "); scanf("%d", &size);
    printf("Head position: "); scanf("%d", &head);
    printf("Requests: "); scanf("%d", &n);
    for(int i = 0; i < n; i++) scanf("%d", &req[i]);
    printf("1.SCAN 2.LOOK: ");
    int ch; scanf("%d", &ch);
    disk_schedule(req, n, head, size, ch == 1);
    return 0;
}
