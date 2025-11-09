#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

#define READERS 5
#define WRITERS 2
#define ITERATIONS 5

int db_data = 0, reader_count = 0;
pthread_mutex_t db_mutex, reader_mutex;

void* reader(void* arg) {
    int id = *(int*)arg;
    for(int iter = 0; iter < ITERATIONS; iter++) {
        pthread_mutex_lock(&reader_mutex);
        reader_count++;
        if(reader_count == 1) pthread_mutex_lock(&db_mutex);
        pthread_mutex_unlock(&reader_mutex);
        
        cout << "Reader " << id << " reading: " << db_data << endl;
        sleep(1);
        
        pthread_mutex_lock(&reader_mutex);
        reader_count--;
        if(reader_count == 0) pthread_mutex_unlock(&db_mutex);
        pthread_mutex_unlock(&reader_mutex);
        sleep(2);
    }
    cout << "Reader " << id << " finished." << endl;
    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;
    for(int iter = 0; iter < ITERATIONS; iter++) {
        pthread_mutex_lock(&db_mutex);
        db_data++;
        cout << "Writer " << id << " writing: " << db_data << endl;
        sleep(2);
        pthread_mutex_unlock(&db_mutex);
        sleep(3);
    }
    cout << "Writer " << id << " finished." << endl;
    return NULL;
}

int main() {
    pthread_t r1, r2, r3, r4, r5;
    pthread_t w1, w2;
    int r_id1 = 1, r_id2 = 2, r_id3 = 3, r_id4 = 4, r_id5 = 5;
    int w_id1 = 1, w_id2 = 2;
    
    pthread_mutex_init(&db_mutex, NULL);
    pthread_mutex_init(&reader_mutex, NULL);
    
    pthread_create(&r1, NULL, reader, &r_id1);
    pthread_create(&r2, NULL, reader, &r_id2);
    pthread_create(&r3, NULL, reader, &r_id3);
    pthread_create(&r4, NULL, reader, &r_id4);
    pthread_create(&r5, NULL, reader, &r_id5);
    
    pthread_create(&w1, NULL, writer, &w_id1);
    pthread_create(&w2, NULL, writer, &w_id2);
    
    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(r3, NULL);
    pthread_join(r4, NULL);
    pthread_join(r5, NULL);
    
    pthread_join(w1, NULL);
    pthread_join(w2, NULL);
    
    pthread_mutex_destroy(&db_mutex);
    pthread_mutex_destroy(&reader_mutex);
    
    return 0;
}