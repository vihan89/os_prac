#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

#define READERS 5
#define WRITERS 2

int data = 0, reader_count = 0;
pthread_mutex_t db_mutex, reader_mutex;

void* reader(void* arg) {
    int id = *(int*)arg;
    while(true) {
        pthread_mutex_lock(&reader_mutex);
        reader_count++;
        if(reader_count == 1) pthread_mutex_lock(&db_mutex);
        pthread_mutex_unlock(&reader_mutex);
        
        cout << "Reader " << id << " reading: " << data << endl;
        sleep(1);
        
        pthread_mutex_lock(&reader_mutex);
        reader_count--;
        if(reader_count == 0) pthread_mutex_unlock(&db_mutex);
        pthread_mutex_unlock(&reader_mutex);
        sleep(2);
    }
    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;
    while(true) {
        pthread_mutex_lock(&db_mutex);
        data++;
        cout << "Writer " << id << " writing: " << data << endl;
        sleep(2);
        pthread_mutex_unlock(&db_mutex);
        sleep(3);
    }
    return NULL;
}

int main() {
    pthread_t r[READERS], w[WRITERS];
    int r_ids[READERS], w_ids[WRITERS];
    
    pthread_mutex_init(&db_mutex, NULL);
    pthread_mutex_init(&reader_mutex, NULL);
    
    for(int i = 0; i < READERS; i++) {
        r_ids[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &r_ids[i]);
    }
    for(int i = 0; i < WRITERS; i++) {
        w_ids[i] = i + 1;
        pthread_create(&w[i], NULL, writer, &w_ids[i]);
    }
    
    for(int i = 0; i < READERS; i++) pthread_join(r[i], NULL);
    for(int i = 0; i < WRITERS; i++) pthread_join(w[i], NULL);
    
    pthread_mutex_destroy(&db_mutex);
    pthread_mutex_destroy(&reader_mutex);
    
    return 0;
}