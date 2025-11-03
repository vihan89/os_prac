/*
8.1 Implement and compare the total head movement for a given sequence of disk 
requests using two specific scheduling algorithms:

System Setup: A disk with cylinders numbered 0 to 499.
Current Head Position: 85
Pending Requests (FIFO order): 10, 229, 39, 400, 18, 145, 120, 480, 20, 250
Algorithms to Implement: C-SCAN and C-LOOK (Assume initial movement is towards 499).
Output: Calculate and display the sequence of cylinder movements and the Average Seek 
Distance for both C-SCAN and C-LOOK. Conclude which algorithm performed better for 
this request set.
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

// C-SCAN Disk Scheduling
int cscan(int requests[], int n, int head, int disk_size) {
    int total_movement = 0;
    int sequence[MAX_REQUESTS + 10];
    int seq_count = 0;
    
    printf("\n===== C-SCAN Algorithm =====\n");
    printf("Initial Head Position: %d\n", head);
    printf("Direction: Towards %d (Right)\n\n", disk_size - 1);
    
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
        
        // Service requests on left (from beginning)
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

// C-LOOK Disk Scheduling
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
    
    // Jump to smallest request on left and service them
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
    int disk_size = 500;  // Cylinders 0 to 499
    int head = 85;
    int requests[] = {10, 229, 39, 400, 18, 145, 120, 480, 20, 250};
    int n = sizeof(requests) / sizeof(requests[0]);
    
    printf("========================================\n");
    printf("  DISK SCHEDULING - C-SCAN vs C-LOOK\n");
    printf("========================================\n\n");
    
    printf("System Configuration:\n");
    printf("  Disk Size: 0 to %d cylinders\n", disk_size - 1);
    printf("  Current Head Position: %d\n", head);
    printf("  Initial Direction: Towards %d\n\n", disk_size - 1);
    
    printf("Pending Requests (FIFO order):\n  ");
    for(int i = 0; i < n; i++) {
        printf("%d", requests[i]);
        if(i < n - 1) printf(", ");
    }
    printf("\n");
    
    // C-SCAN Algorithm
    int cscan_movement = cscan(requests, n, head, disk_size);
    printf("\nTotal Head Movement (C-SCAN): %d cylinders\n", cscan_movement);
    printf("Average Seek Distance (C-SCAN): %.2f cylinders\n", (float)cscan_movement / n);
    
    printf("\n========================================\n");
    
    // C-LOOK Algorithm
    int clook_movement = clook(requests, n, head);
    printf("\nTotal Head Movement (C-LOOK): %d cylinders\n", clook_movement);
    printf("Average Seek Distance (C-LOOK): %.2f cylinders\n", (float)clook_movement / n);
    
    // Comparison
    printf("\n========================================\n");
    printf("         COMPARISON RESULTS\n");
    printf("========================================\n");
    printf("Algorithm\tTotal Movement\tAvg Seek Distance\n");
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
