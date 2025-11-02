/*
8.3 A disk drive has 5000 cylinders, numbered 0 to 4999. The drive is currently 
serving a request at cylinder 143. The previous request was at cylinder 125. 
The queue of pending requests, in FIFO order, is: 
86, 1470, 913, 1774, 948, 1509, 1022, 1750, 130.

Calculate the average seek length (in cylinders) that the disk arm moves to 
satisfy all of the pending requests for:
C-SCAN (Assume the head is currently moving toward 4999) and 
C-LOOK (Assume the head is currently moving toward 4999).
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

// C-SCAN Disk Scheduling Algorithm
int cscan(int requests[], int n, int head, int disk_size) {
    int total_movement = 0;
    int sequence[MAX_REQUESTS + 10];
    int seq_count = 0;
    
    printf("\n===== C-SCAN Algorithm =====\n");
    printf("Initial Head Position: %d\n", head);
    printf("Direction: Towards 4999 (Right)\n\n");
    
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
    
    // First service requests on right (towards disk end)
    for(int i = 0; i < right_count; i++) {
        sequence[seq_count++] = right[i];
    }
    
    // Go to end of disk (if there were requests on right)
    if(right_count > 0) {
        sequence[seq_count++] = disk_size - 1;
    }
    
    // Jump to beginning of disk (0)
    if(left_count > 0) {
        sequence[seq_count++] = 0;
        
        // Service requests on left (from beginning, ascending)
        for(int i = 0; i < left_count; i++) {
            sequence[seq_count++] = left[i];
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

// C-LOOK Disk Scheduling Algorithm
int clook(int requests[], int n, int head) {
    int total_movement = 0;
    int sequence[MAX_REQUESTS + 10];
    int seq_count = 0;
    
    printf("\n===== C-LOOK Algorithm =====\n");
    printf("Initial Head Position: %d\n", head);
    printf("Direction: Towards highest request (Right)\n\n");
    
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
    
    // First service requests on right (ascending order)
    for(int i = 0; i < right_count; i++) {
        sequence[seq_count++] = right[i];
    }
    
    // Jump to smallest request on left and service them (ascending)
    if(left_count > 0) {
        // Service requests on left (from smallest to largest)
        for(int i = 0; i < left_count; i++) {
            sequence[seq_count++] = left[i];
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
    
    printf("========================================\n");
    printf("  DISK SCHEDULING - C-SCAN vs C-LOOK\n");
    printf("========================================\n\n");
    
    printf("System Configuration:\n");
    printf("  Disk Size: 0 to %d cylinders\n", disk_size - 1);
    printf("  Current Head Position: %d\n", head);
    printf("  Previous Request: %d\n", prev_request);
    printf("  Current Direction: Towards 4999 (Right)\n\n");
    
    printf("Pending Requests (FIFO order):\n  ");
    for(int i = 0; i < n; i++) {
        printf("%d", requests[i]);
        if(i < n - 1) printf(", ");
    }
    printf("\n");
    
    // C-SCAN Algorithm
    int cscan_movement = cscan(requests, n, head, disk_size);
    printf("\nTotal Head Movement (C-SCAN): %d cylinders\n", cscan_movement);
    printf("Average Seek Length (C-SCAN): %.2f cylinders\n", (float)cscan_movement / n);
    
    printf("\n========================================\n");
    
    // C-LOOK Algorithm
    int clook_movement = clook(requests, n, head);
    printf("\nTotal Head Movement (C-LOOK): %d cylinders\n", clook_movement);
    printf("Average Seek Length (C-LOOK): %.2f cylinders\n", (float)clook_movement / n);
    
    // Comparison
    printf("\n========================================\n");
    printf("         COMPARISON RESULTS\n");
    printf("========================================\n");
    printf("Algorithm\tTotal Movement\tAvg Seek Length\n");
    printf("--------------------------------------------------------\n");
    printf("C-SCAN\t\t%d\t\t%.2f\n", cscan_movement, (float)cscan_movement / n);
    printf("C-LOOK\t\t%d\t\t%.2f\n", clook_movement, (float)clook_movement / n);
    printf("--------------------------------------------------------\n");
    
    if(cscan_movement < clook_movement) {
        printf("\nConclusion: C-SCAN performed BETTER!\n");
        printf("C-SCAN saved %d cylinders compared to C-LOOK.\n", clook_movement - cscan_movement);
    } else if(clook_movement < cscan_movement) {
        printf("\nConclusion: C-LOOK performed BETTER!\n");
        printf("C-LOOK saved %d cylinders compared to C-SCAN.\n", cscan_movement - clook_movement);
    } else {
        printf("\nConclusion: Both algorithms performed EQUALLY!\n");
    }
    
    printf("========================================\n");
    
    return 0;
}
