

#include <iostream>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;

#define FILENAME "shared_data.txt"
#define NUM_ITEMS 10

sem_t data_available;
pthread_mutex_t file_mutex;

void* producer(void* arg) {
    srand(time(NULL));
    
    for(int i = 0; i < NUM_ITEMS; i++) {
        int data = rand() % 10;
        

        pthread_mutex_lock(&file_mutex);
        

        ofstream file(FILENAME);
        if(file.is_open()) {
            file << data;
            file.close();
            cout << "📝 Producer writes: " << data << endl;
        }
        else {
            cout << "❌ Producer: Error opening file!" << endl;
        }
        
        pthread_mutex_unlock(&file_mutex);
        

        sem_post(&data_available);
        
        sleep(1);
    }
    
    cout << "\n✓ Producer finished writing." << endl;
    return NULL;
}

void* consumer(void* arg) {
    for(int i = 0; i < NUM_ITEMS; i++) {

        sem_wait(&data_available);
        

        pthread_mutex_lock(&file_mutex);
        

        ifstream file(FILENAME);
        if(file.is_open()) {
            int data;
            file >> data;
            file.close();
            cout << "📖 Consumer reads: " << data << endl;
        }
        else {
            cout << "❌ Consumer: Error opening file!" << endl;
        }
        
        pthread_mutex_unlock(&file_mutex);
        
        sleep(2);
    }
    
    cout << "\n✓ Consumer finished reading." << endl;
    return NULL;
}

int main() {
    pthread_t prod, cons;
    
    cout << "========================================" << endl;
    cout << "  PRODUCER-CONSUMER WITH FILE I/O" << endl;
    cout << "========================================" << endl;
    cout << "File: " << FILENAME << endl;
    cout << "Items: " << NUM_ITEMS << endl;
    cout << "========================================\n" << endl;
    

    sem_init(&data_available, 0, 0);
    pthread_mutex_init(&file_mutex, NULL);
    

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    

    sem_destroy(&data_available);
    pthread_mutex_destroy(&file_mutex);
    
    cout << "\n========================================" << endl;
    cout << "  OPERATIONS COMPLETED!" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

