#include <stdint.h>
#include <pthread.h>

#define NUMITEMS 8

uint64_t buffer[NUMITEMS];
int buffer_size = 0;

pthread_mutex_t stopped_activity_mutex;
pthread_cond_t buffer_is_full_cond;
pthread_cond_t buffer_is_empty_cond;

void *producer(void *args){
    int produced_items[NUMITEMS];
    for(int item = 0; item < NUMITEMS; item++){
        produced_items[item] = item;
    }

    // Sleep

    for(int item = 0; item < NUMITEMS; item ++){
        buffer[buffer_size] = produced_items[item];
        buffer_size ++;
    }
    
}

void *consumer(void *args){
    
}


int main(){
    pthread_t prod_thread, cons_thread;

    pthread_mutex_init(&stopped_activity_mutex, NULL);
    pthread_cond_init(&buffer_is_full_cond, NULL);
    pthread_cond_init(&buffer_is_empty_cond, NULL);

    if(pthread_create(&prod_thread, NULL, producer, NULL) != 0){
        printf("Failed to create thread");
        exit(0);
    }
    
    if(pthread_create(&cons_thread, NULL, consumer, NULL) != 0){
        printf("Failed to create thread");
        exit(0);
    }

    if(pthread_join(prod_thread, NULL) != 0){
        printf("Failed to join thread");
        exit(0);
    }
    if(pthread_join(cons_thread, NULL) != 0){
        printf("Failed to join thread");
        exit(0);
    }
    
    pthread_mutex_destroy(&stopped_activity_mutex);
    pthread_cond_destroy(&buffer_is_full_cond);
    pthread_cond_destroy(&buffer_is_empty_cond);
    return 0;
}