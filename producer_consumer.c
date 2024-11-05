#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define NUMITEMS 8
#define MAX_PRODUCED_ITEMS 100

uint64_t queue[NUMITEMS];
int num_items_in_queue = 0;

pthread_mutex_t stopped_activity_mutex;
pthread_cond_t queue_is_not_full_cond;
pthread_cond_t queue_is_not_empty_cond;

void *producer(void *args){
    for(int i = 0; i < MAX_PRODUCED_ITEMS; i++){
        pthread_mutex_lock(&stopped_activity_mutex);
        while (num_items_in_queue == NUMITEMS) {
            pthread_cond_wait(&queue_is_not_full_cond, &stopped_activity_mutex);
        }
        
        int item = i % NUMITEMS;
        queue[item] = i;
        printf("Produced %d\n", i);
        num_items_in_queue ++;

        pthread_cond_signal(&queue_is_not_empty_cond);
        pthread_mutex_unlock(&stopped_activity_mutex);

        float time_to_sleep = ((float)rand() / RAND_MAX) * 1000000;
        usleep(time_to_sleep);
    }
    
}

void *consumer(void *args){
    for (int i = 0; i < MAX_PRODUCED_ITEMS; i++) {
        pthread_mutex_lock(&stopped_activity_mutex);

        while (num_items_in_queue == 0) {
            pthread_cond_wait(&queue_is_not_empty_cond, &stopped_activity_mutex);
        }

        int item = queue[i % NUMITEMS];
        printf("Consumed %d\n", item);
        num_items_in_queue --;
    
        pthread_cond_signal(&queue_is_not_full_cond);
        pthread_mutex_unlock(&stopped_activity_mutex);

        float time_to_sleep = ((float)rand() / RAND_MAX) * 1000000;
        usleep(time_to_sleep);
    }
    return NULL;
}


int main(){
    pthread_t prod_thread, cons_thread;
    srand(42);

    pthread_mutex_init(&stopped_activity_mutex, NULL);
    pthread_cond_init(&queue_is_not_full_cond, NULL);
    pthread_cond_init(&queue_is_not_empty_cond, NULL);

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
    pthread_cond_destroy(&queue_is_not_full_cond);
    pthread_cond_destroy(&queue_is_not_empty_cond);
    return 0;
}
