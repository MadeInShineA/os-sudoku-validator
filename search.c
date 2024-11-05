#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define N 9
#define CHAR_TO_SEARCH 'A'

typedef struct{
    int column_number;
} parameters;

bool char_is_found_bool = false;
pthread_mutex_t char_is_found_mutex;
char matrix[N][N] = {
    {'G', 'X', 'M', 'B', 'J', 'L', 'Q', 'T', 'R'},
    {'P', 'E', 'K', 'N', 'F', 'Z', 'S', 'Y', 'W'},
    {'V', 'U', 'D', 'A', 'O', 'C', 'H', 'I', 'B'},
    {'L', 'Q', 'G', 'W', 'X', 'P', 'R', 'K', 'S'},
    {'F', 'T', 'C', 'M', 'E', 'J', 'U', 'V', 'F'},
    {'N', 'H', 'Y', 'F', 'R', 'G', 'T', 'L', 'O'},
    {'S', 'V', 'Z', 'C', 'D', 'F', 'B', 'W', 'U'},
    {'I', 'P', 'L', 'R', 'N', 'K', 'X', 'G', 'J'},
    {'O', 'J', 'F', 'Q', 'H', 'Y', 'M', 'E', 'D'}
};

void* check_column_n(void *args) {
    int n = *(int *)args;

    for (int row = 0; row < N; row++) {

        // Check if another thread already found the number
        if (char_is_found_bool) {
            return NULL;
        }

        char current_char = matrix[row][n];
        if (current_char == CHAR_TO_SEARCH){
            pthread_mutex_lock(&char_is_found_mutex);
            char_is_found_bool = true;
            printf("Number char in column %d\n", n);
            pthread_mutex_unlock(&char_is_found_mutex);
            return NULL;
        }
    }
    return NULL;
}


int main(){    
    pthread_t threads[N];    

    for(int i = 0; i < N; i++){
        if(pthread_create(&threads[i], NULL, check_column_n, (void *) &i) != 0){
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
    pthread_mutex_destroy(&char_is_found_mutex);

    if (!char_is_found_bool){
        printf("The character isn't in the matrix");
    }
    

    return 0;
}
