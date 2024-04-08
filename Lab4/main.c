#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <malloc.h>
#include <time.h>

#define NUM_THREADS 2
#define SIZE 100000000

typedef struct {
    int* data;
    int first;  
    int last;    
} ThreadData;

DWORD WINAPI incrementArr(void *args){
    ThreadData* data = (ThreadData*) args;

    for(int i = data->first; i<= data->last; i++){
        data->data[i]+=1;
    }
    return 0;
}

int main() {
    // Init data
    // int size = 10000000;
    int size_to_process_by_thread = SIZE/NUM_THREADS;
    int *data = malloc(SIZE*sizeof(int));
    for(int i=0;i<SIZE;i++){
        data[i] = 0;
    }

        printf("Data before change: ");
    for(int i=0; i<10; i++){
        printf("%d ", data[i]);
    }
    printf("\n");

    HANDLE threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];
    DWORD id;

    clock_t start = clock();
    for(int i=0;i<NUM_THREADS;i++){   
        threadData[i].data = data;
        threadData[i].first = i*size_to_process_by_thread;
        if(i==NUM_THREADS-1)threadData[i].last = SIZE - 1; 
        else threadData[i].last = (i+1) * size_to_process_by_thread - 1;
        
        threads[i] = CreateThread(
            NULL,
            0,
            incrementArr,
            (void*)&threadData[i],
            0,
            &id
        );
    }

    WaitForMultipleObjects(NUM_THREADS, threads, 1, INFINITE);
    clock_t end = clock();
    double time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Data after change: ");
    for(int i=0; i<10; i++){
        printf("%d ", data[i]);
    }
    printf("\n");

    printf("(Number of threads: %d) (Size: %d) (Time: %.4fs) \n", NUM_THREADS, SIZE, time);


    free(data);
    return 0;
}