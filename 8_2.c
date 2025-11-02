/*
8.2 A disk drive has 5000 cylinders, numbered 0 to 4999. The drive is currently 
serving a request at cylinder 143. The previous request was at cylinder 125. 
The queue of pending requests, in FIFO order, is: 
86, 1470, 913, 1774, 948, 1509, 1022, 1750, 130.

Calculate the average seek length (in cylinders) that the disk arm moves to 
satisfy all of the pending requests for:
SCAN (Assume the head is currently moving toward 4999) and 
LOOK (Assume the head is currently moving toward 4999).
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_REQUESTS 100

// Function to sort array
void sort(int arr[], int n) {
    for(int i = 0; i < n - 1; i++) {
        for(int j = 0; j < n - i - 1; j++) {
            if(arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// SCAN Disk Scheduling Algorithm
int scan(int requests[], int n, int head, int disk_size, int direction) {
    int total_movement = 0;
    int sequence[MAX_REQUESTS + 10];
    int seq_count = 0;
    
    printf("\n===== SCAN Algorithm =====\n");
    printf("Initial Head Position: %d\n", head);
    printf("Direction: Towards %s\n\n", direction == 1 ? "4999 (Right)" : "0 (Left)");
    
    // Separate requests into left and right of head
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int left_count = 0, right_count = 0;
    
    for(int i = 0; i < n; i++) {
        if(requests[i] < head) {
            left[left_count++] = requests[i];
        } else {
            right[right_count++] = requests[i];
        }
    }
    
    // Sort both arrays
    sort(left, left_count);
    sort(right, right_count);
    
    // Add current head position
    sequence[seq_count++] = head;
    
    if(direction == 1) {
        // Moving towards right (4999)
        // Service all requests on right
        for(int i = 0; i < right_count; i++) {
            sequence[seq_count++] = right[i];
        }
        
        // Go to end of disk
        sequence[seq_count++] = disk_size - 1;
        
        // Reverse direction and service left requests (descending)
        for(int i = left_count - 1; i >= 0; i--) {
            sequence[seq_count++] = left[i];
        }
    } else {
        // Moving towards left (0)
        // Service all requests on left (descending)
        for(int i = left_count - 1; i >= 0; i--) {
            sequence[seq_count++] = left[i];
        }
        
        // Go to beginning of disk
        sequence[seq_count++] = 0;
        
        // Reverse direction and service right requests (ascending)
        for(int i = 0; i < right_count; i++) {
            sequence[seq_count++] = right[i];
        }
    }
    
    // Calculate total head movement
    printf("Seek Sequence:\n");
    for(int i = 0; i < seq_count; i++) {
        if(i > 0) {
            total_movement += abs(sequence[i] - sequence[i - 1]);
            printf("%d -> ", sequence[i - 1]);
        }
    }
    printf("%d\n\n", sequence[seq_count - 1]);
    
    printf("Detailed Movement:\n");
    for(int i = 1; i < seq_count; i++) {
        int movement = abs(sequence[i] - sequence[i - 1]);
        printf("%d to %d: %d cylinders\n", sequence[i - 1], sequence[i], movement);
    }
    
    return total_movement;
}

// LOOK Disk Scheduling Algorithm
int look(int requests[], int n, int head, int direction) {
    int total_movement = 0;
    int sequence[MAX_REQUESTS + 10];
    int seq_count = 0;
    
    printf("\n===== LOOK Algorithm =====\n");
    printf("Initial Head Position: %d\n", head);
    printf("Direction: Towards %s\n\n", direction == 1 ? "4999 (Right)" : "0 (Left)");
    
    // Separate requests into left and right of head
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int left_count = 0, right_count = 0;
    
    for(int i = 0; i < n; i++) {
        if(requests[i] < head) {
            left[left_count++] = requests[i];
        } else {
            right[right_count++] = requests[i];
        }
    }
    
    // Sort both arrays
    sort(left, left_count);
    sort(right, right_count);
    
    // Add current head position
    sequence[seq_count++] = head;
    
    if(direction == 1) {
        // Moving towards right (highest request)
        // Service all requests on right
        for(int i = 0; i < right_count; i++) {
            sequence[seq_count++] = right[i];
        }
        
        // Reverse direction and service left requests (descending)
        for(int i = left_count - 1; i >= 0; i--) {
            sequence[seq_count++] = left[i];
        }
    } else {
        // Moving towards left (lowest request)
        // Service all requests on left (descending)
        for(int i = left_count - 1; i >= 0; i--) {
            sequence[seq_count++] = left[i];
        }
        
        // Reverse direction and service right requests (ascending)
        for(int i = 0; i < right_count; i++) {
            sequence[seq_count++] = right[i];
        }
    }
    
    // Calculate total head movement
    printf("Seek Sequence:\n");
    for(int i = 0; i < seq_count; i++) {
        if(i > 0) {
            total_movement += abs(sequence[i] - sequence[i - 1]);
            printf("%d -> ", sequence[i - 1]);
        }
    }
    printf("%d\n\n", sequence[seq_count - 1]);
    
    printf("Detailed Movement:\n");
    for(int i = 1; i < seq_count; i++) {
        int movement = abs(sequence[i] - sequence[i - 1]);
        printf("%d to %d: %d cylinders\n", sequence[i - 1], sequence[i], movement);
    }
    
    return total_movement;
}

int main() {
    // Given data
    int disk_size = 5000;  // Cylinders 0 to 4999
    int head = 143;
    int prev_request = 125;
    int requests[] = {86, 1470, 913, 1774, 948, 1509, 1022, 1750, 130};
    int n = sizeof(requests) / sizeof(requests[0]);
    
    // Determine direction (head > prev means moving right)
    int direction = (head > prev_request) ? 1 : 0;
    
    printf("========================================\n");
    printf("  DISK SCHEDULING - SCAN vs LOOK\n");
    printf("========================================\n\n");
    
    printf("System Configuration:\n");
    printf("  Disk Size: 0 to %d cylinders\n", disk_size - 1);
    printf("  Current Head Position: %d\n", head);
    printf("  Previous Request: %d\n", prev_request);
    printf("  Current Direction: Towards %s\n\n", direction == 1 ? "4999 (Right)" : "0 (Left)");
    
    printf("Pending Requests (FIFO order):\n  ");
    for(int i = 0; i < n; i++) {
        printf("%d", requests[i]);
        if(i < n - 1) printf(", ");
    }
    printf("\n");
    
    // SCAN Algorithm
    int scan_movement = scan(requests, n, head, disk_size, direction);
    printf("\nTotal Head Movement (SCAN): %d cylinders\n", scan_movement);
    printf("Average Seek Length (SCAN): %.2f cylinders\n", (float)scan_movement / n);
    
    printf("\n========================================\n");
    
    // LOOK Algorithm
    int look_movement = look(requests, n, head, direction);
    printf("\nTotal Head Movement (LOOK): %d cylinders\n", look_movement);
    printf("Average Seek Length (LOOK): %.2f cylinders\n", (float)look_movement / n);
    
    // Comparison
    printf("\n========================================\n");
    printf("         COMPARISON RESULTS\n");
    printf("========================================\n");
    printf("Algorithm\tTotal Movement\tAvg Seek Length\n");
    printf("--------------------------------------------------------\n");
    printf("SCAN\t\t%d\t\t%.2f\n", scan_movement, (float)scan_movement / n);
    printf("LOOK\t\t%d\t\t%.2f\n", look_movement, (float)look_movement / n);
    printf("--------------------------------------------------------\n");
    
    if(scan_movement < look_movement) {
        printf("\nConclusion: SCAN performed BETTER!\n");
        printf("SCAN saved %d cylinders compared to LOOK.\n", look_movement - scan_movement);
    } else if(look_movement < scan_movement) {
        printf("\nConclusion: LOOK performed BETTER!\n");
        printf("LOOK saved %d cylinders compared to SCAN.\n", scan_movement - look_movement);
    } else {
        printf("\nConclusion: Both algorithms performed EQUALLY!\n");
    }
    
    printf("========================================\n");
    
    return 0;
}
