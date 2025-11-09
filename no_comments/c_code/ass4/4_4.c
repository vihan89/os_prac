#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

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
        
        printf("Reader %d reading: %d\n", id, db_data);
        sleep(1);
        
        pthread_mutex_lock(&reader_mutex);
        reader_count--;
        if(reader_count == 0) pthread_mutex_unlock(&db_mutex);
        pthread_mutex_unlock(&reader_mutex);
        sleep(2);
    }
    printf("Reader %d finished.\n", id);
    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;
    for(int iter = 0; iter < ITERATIONS; iter++) {
        pthread_mutex_lock(&db_mutex);
        db_data++;
        printf("Writer %d writing: %d\n", id, db_data);
        sleep(2);
        pthread_mutex_unlock(&db_mutex);
        sleep(3);
    }
    printf("Writer %d finished.\n", id);
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
