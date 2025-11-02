/*
4.8 Resource Allocation Problem: 
Assume that a finite number of resources of a single resource type must be managed. 
Processes may ask for a number of these resources and will return them once finished. 
As an example, many commercial software packages provide a given number of licenses, 
indicating the number of applications that may run concurrently. 
Using a semaphore or mutex lock, fix the race condition. When a process wishes to obtain 
a number of resources, it invokes the decrease_count() function: decrease available 
resources by count resources and return 0 if sufficient resources available. 
When a process wants to return a number of resources, it calls the increase_count() 
function: increase available resources by count.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_RESOURCES 10
#define NUM_PROCESSES 15

int available_resources = NUM_RESOURCES;
pthread_mutex_t resource_mutex;

// Decrease available resources by count
int decrease_count(int count, int process_id) {
    pthread_mutex_lock(&resource_mutex);
    
    if(available_resources >= count) {
        available_resources -= count;
        printf("Process %d: Acquired %d resource(s). Available: %d\n", 
               process_id, count, available_resources);
        pthread_mutex_unlock(&resource_mutex);
        return 0;  // Success
    } else {
        printf("Process %d: Cannot acquire %d resource(s). Available: %d\n", 
               process_id, count, available_resources);
        pthread_mutex_unlock(&resource_mutex);
        return -1;  // Failure
    }
}

// Increase available resources by count
int increase_count(int count, int process_id) {
    pthread_mutex_lock(&resource_mutex);
    
    available_resources += count;
    printf("Process %d: Released %d resource(s). Available: %d\n", 
           process_id, count, available_resources);
    
    pthread_mutex_unlock(&resource_mutex);
    return 0;
}

void* process_thread(void* arg) {
    int process_id = *(int*)arg;
    int resources_needed = (rand() % 3) + 1;  // Need 1-3 resources
    
    sleep(rand() % 2);
    
    printf("Process %d: Requesting %d resource(s)...\n", process_id, resources_needed);
    
    // Try to acquire resources
    while(decrease_count(resources_needed, process_id) != 0) {
        printf("Process %d: Waiting for resources...\n", process_id);
        sleep(1);
    }
    
    // Use resources
    printf("Process %d: Using %d resource(s)...\n", process_id, resources_needed);
    sleep(rand() % 3 + 1);
    
    // Release resources
    increase_count(resources_needed, process_id);
    printf("Process %d: Finished\n", process_id);
    
    return NULL;
}

int main() {
    pthread_t processes[NUM_PROCESSES];
    int process_ids[NUM_PROCESSES];
    
    srand(time(NULL));
    
    // Initialize mutex
    pthread_mutex_init(&resource_mutex, NULL);
    
    printf("=== Resource Allocation Problem ===\n");
    printf("Total resources available: %d\n", NUM_RESOURCES);
    printf("Number of processes: %d\n\n", NUM_PROCESSES);
    
    // Create process threads
    for(int i = 0; i < NUM_PROCESSES; i++) {
        process_ids[i] = i + 1;
        pthread_create(&processes[i], NULL, process_thread, &process_ids[i]);
    }
    
    // Wait for all processes to complete
    for(int i = 0; i < NUM_PROCESSES; i++) {
        pthread_join(processes[i], NULL);
    }
    
    printf("\n=== All processes completed ===\n");
    printf("Final available resources: %d\n", available_resources);
    
    if(available_resources == NUM_RESOURCES) {
        printf("SUCCESS: All resources returned correctly!\n");
    } else {
        printf("ERROR: Resource leak detected!\n");
    }
    
    // Cleanup
    pthread_mutex_destroy(&resource_mutex);
    
    return 0;
}
