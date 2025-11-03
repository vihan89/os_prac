

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define NUM_READERS 5
#define NUM_WRITERS 2

int shared_data = 0;
int reader_count = 0;
pthread_mutex_t db_mutex;
pthread_mutex_t reader_mutex;

void* reader(void* arg) {
    int reader_id = *(int*)arg;
    
    while(true) {

        pthread_mutex_lock(&reader_mutex);
        reader_count++;
        if(reader_count == 1) {

            pthread_mutex_lock(&db_mutex);
        }
        pthread_mutex_unlock(&reader_mutex);
        

        cout << "📖 Reader " << reader_id << " reading data: " << shared_data << endl;
        sleep(1);
        cout << "✓ Reader " << reader_id << " finished reading" << endl;

        

        pthread_mutex_lock(&reader_mutex);
        reader_count--;
        if(reader_count == 0) {

            pthread_mutex_unlock(&db_mutex);
        }
        pthread_mutex_unlock(&reader_mutex);
        
        sleep(2);
    }
    
    return NULL;
}

void* writer(void* arg) {
    int writer_id = *(int*)arg;
    
    while(true) {

        pthread_mutex_lock(&db_mutex);
        

        shared_data++;
        cout << "✍️  Writer " << writer_id << " writing data: " << shared_data << endl;
        sleep(2);
        cout << "✓ Writer " << writer_id << " finished writing" << endl;

        

        pthread_mutex_unlock(&db_mutex);
        
        sleep(3);
    }
    
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];
    
    cout << "========================================" << endl;
    cout << "  READERS-WRITERS PROBLEM" << endl;
    cout << "  (Readers Priority)" << endl;
    cout << "========================================" << endl;
    cout << "Readers: " << NUM_READERS << endl;
    cout << "Writers: " << NUM_WRITERS << endl;
    cout << "========================================\n" << endl;
    

    pthread_mutex_init(&db_mutex, NULL);
    pthread_mutex_init(&reader_mutex, NULL);
    

    for(int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }
    

    for(int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }
    

    for(int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for(int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }
    

    pthread_mutex_destroy(&db_mutex);
    pthread_mutex_destroy(&reader_mutex);
    
    return 0;
}

