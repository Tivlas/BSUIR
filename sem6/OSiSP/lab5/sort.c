#include "sort.h"

#include <stdio.h>
#include <stdlib.h>

void merge(size_t l, size_t q, size_t r, int* const A, int* L, int* R) {
    size_t n1 = q - l + 1;
    size_t n2 = r - q;
    for (size_t i = 0; i < n1; i++) {
        L[i] = A[l + i];
    }
    for (size_t i = 0; i < n2; i++) {
        R[i] = A[q + 1 + i];
    }
    L[n1] = __INT_MAX__;
    R[n2] = __INT_MAX__;
    int i = 0;
    int j = 0;
    for (size_t k = l; k <= r; k++) {
        if (L[i] <= R[j]) {
            A[k] = L[i++];
        } else {
            A[k] = R[j++];
        }
    }
}

int comp(const void* i, const void* j) { return *(int*)i - *(int*)j; }

void* qsort_wrapper(void* args) {
    struct SortArgs* merge_args = args;
    int l = merge_args->l;
    int r = merge_args->r;
    int* array = merge_args->A;
    if (l < r) {
        qsort(array + l, r - l + 1, sizeof(int), comp);
    }
    return NULL;
}