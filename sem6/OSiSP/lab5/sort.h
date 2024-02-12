#ifndef SORT_H
#define SORT_H
#include <stddef.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

void merge(size_t l, size_t q, size_t r, int* const A, int* L, int* R);
void* qsort_wrapper(void* args);
int comp(const void* i, const void* j);

struct SortArgs {
    int l;
    int r;
    int* A;
};

#endif  // SORT_H