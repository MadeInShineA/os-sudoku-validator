#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

float mean = 0.0;
int max = 0;
int min = 0;

typedef struct thread_data{
    int length;
    int *numbers;
} thread_data_t;

void *mean_function(void *arg){
    thread_data_t *data = (thread_data_t *)arg;
    int length = data->length;
    int *numbers = data->numbers;
    int sum = 0;
    
    for(int i = 0; i < length; i++){
        sum += data->numbers[i];
    }
    mean = (float)sum / length;
    pthread_exit(0);
}

void *max_function(void *arg){
    thread_data_t *data = (thread_data_t *)arg;
    int length = data->length;
    int *numbers = data->numbers;
    max = numbers[0];
    
    for (int i = 0; i < length; i++){
        if(numbers[i]>max){
            max = numbers[i];
        }
    }
    pthread_exit(0);
}

void *min_function(void *arg){
    thread_data_t *data = (thread_data_t *)arg;
    int length = data->length;
    int *numbers = data->numbers;
    min = numbers[0];
    
    for (int i = 0; i < length; i++){
        if(numbers[i]<min){
            min = numbers[i];
        }
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if(argc == 1){
        printf("Needed at least 1 argument");
        exit(0);
    }

    int number_of_int = argc - 1;
    int numbers[number_of_int];
    thread_data_t data = {
        number_of_int,
        numbers
    };

    for(int i=0; i < data.length; i++){
        data.numbers[i] = atoi(argv[i+1]);
    }

    pthread_t thread_1, thread_2, thread_3;

    pthread_create(&thread_1, NULL, mean_function, &data);
    pthread_create(&thread_2, NULL, max_function, &data);
    pthread_create(&thread_3, NULL, min_function, &data);

    pthread_join(thread_1, NULL);
    printf("The average value is %.2f\n", mean);

    pthread_join(thread_2, NULL);
    printf("The maximum value is %d\n", max);

    pthread_join(thread_3, NULL);
    printf("The minimum value is %d\n", min);
    
    return 0;
}