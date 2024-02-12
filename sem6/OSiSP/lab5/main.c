#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#include "sort.h"
#define MAX_THREADS 12
#define BILLION 1000000000L

void print(const int* const array, size_t array_size) {
    for (size_t i = 0; i < array_size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void parallel_merge_sort(int* array, size_t n, size_t threads_num) {
    pthread_t* threads = (pthread_t*)malloc(threads_num * sizeof(pthread_t));
    struct SortArgs* args =
        (struct SortArgs*)malloc(threads_num * sizeof(struct SortArgs));
    int* L = (int*)malloc(n * sizeof(int));
    int* R = (int*)malloc(n * sizeof(int));

    uint64_t diff;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW , &start);
    size_t range_size =
        ((size_t)ceil((double)n / threads_num) * threads_num) / threads_num;

    for (size_t i = 0; i < threads_num; i++) {
        args[i].l = i * range_size;
        args[i].r = MIN(n - 1, (i + 1) * range_size - 1);
        args[i].A = array;
        pthread_create(&threads[i], NULL, qsort_wrapper, (void*)&args[i]);
    }

    for (size_t i = 0; i < threads_num; i++) {
        pthread_join(threads[i], NULL);
    }

    while (range_size < n) {
        for (size_t i = 0; i < (size_t)ceil((double)n / (range_size << 1));
             i++) {
            size_t l = i * (range_size << 1),
                   q = MIN(n - 1, i * (range_size << 1) + range_size - 1),
                   r = MIN(n - 1, (i + 1) * (range_size << 1) - 1);
            merge(l, q, r, array, L, R);
        }
        range_size <<= 1;
    }

    clock_gettime(CLOCK_MONOTONIC_RAW , &end);
    diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("parallel qsort (without memory allocation) time = %ld ns\n", diff);

    free(threads);
    free(args);
    free(L);
    free(R);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: ./program <array_size> <num_threads> <print? (0, 1)>\n");
        return 1;
    }

    size_t size = atoi(argv[1]);
    int threads_num = atoi(argv[2]);
    int need_print = atoi(argv[3]);

    if (size <= 0 || threads_num <= 0 || (need_print != 0 && need_print != 1)) {
        printf(
            "Invalid input. Array size and number of threads must be positive "
            "integers and third parameter 0 or 1.\n");
        return 1;
    }

    if (threads_num > MAX_THREADS) {
        printf(
            "Number of threads exceeds the maximum limit. Using maximum "
            "threads: %d\n",
            MAX_THREADS);
        threads_num = MAX_THREADS;
    }

    int* A = (int*)malloc(size * sizeof(int));
    int* B = (int*)malloc(size * sizeof(int));

    for (size_t i = 0; i < size; i++) {
        A[i] = rand() % 100;
        B[i] = A[i];
    }

    uint64_t diff;
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC_RAW , &start);
    parallel_merge_sort(A, size, threads_num);
    clock_gettime(CLOCK_MONOTONIC_RAW , &end);
    diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("parallel qsort time = %ld ns\n", diff);

    clock_gettime(CLOCK_MONOTONIC_RAW , &start);
    qsort(B, size, sizeof(int), comp);
    clock_gettime(CLOCK_MONOTONIC_RAW , &end);
    diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("qsort time = %ld ns\n", diff);

    if (need_print) {
        print(A, size);
        print(B, size);
    }
    for (size_t i = 0; i < size; i++) {
        if (A[i] != B[i]) {
            printf("WRONG...");
            break;
        }
    }

    free(A);
    free(B);
    return 0;
}
