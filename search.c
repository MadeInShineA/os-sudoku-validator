#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>


#define N 9
#define NUMBER_TO_SEARCH 3

typedef struct{
    int column_number;
} parameters;

bool number_is_found_bool = false;
pthread_cond_t number_is_found_cond;
int matrix[N][N];

void* check_column_n(void *args){
    int n = *(int *)args;    
    for(int row = 0; row < N; row++){
        int current_number = matrix[row][n];
        if(current_number == NUMBER_TO_SEARCH){
            // Lock
            number_is_found_bool = true;
            pthread_cond_signal(&number_is_found_cond); 
        }
    }
}

void* check_column_n(void *args) {
    int n = *(int *)args;

    for (int row = 0; row < N; row++) {
        pthread_mutex_lock(&number_is_found_mutex);

        // Check if another thread already found the number
        if (number_is_found_bool) {
            pthread_mutex_unlock(&number_is_found_mutex);
            return NULL; // Stop searching
        }

        int current_number = matrix[row][n];
        if (current_number == NUMBER_TO_SEARCH) {
            number_is_found_bool = true;
            pthread_cond_broadcast(&number_is_found_cond); // Notify all threads
            pthread_mutex_unlock(&number_is_found_mutex);
            return NULL;
        }

        pthread_mutex_unlock(&number_is_found_mutex);
    }
    return NULL;
}


int main(){    
    pthread_t threads[N];
    int threads_numbers[N];

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            matrix[i][j] = i + j;
        }
        threads_numbers[i] = i;
    }
    
    pthread_cond_init(&number_is_found_cond, NULL);

    for(int i = 0; i < N; i++){
        if(pthread_create(&threads[i], NULL, check_column_n, (void *) &threads_numbers[i]) != 0){
            printf("Failed to create thread");
            exit(0);
        }
    }

    for(int i = 0; i < N; i++){
        if(pthread_join(threads[i], NULL) != 0){
            printf("Failed to join thread");
            exit(0);
        }
    }

    pthread_cond_destroy(&number_is_found_cond);

    return 0;
}
