#include "HEAP.h"

struct heap_s {
    Item *A;
    int heapsize;
};

static int LEFT(int i) {
    return 2*i + 1;
}

static int RIGHT(int i) {
    return 2*i + 2;
}

static int PARENT(int i) {
    return (i-1)/2;
}

static void Swap(Heap h, int a, int b) {
    Item tmp;
    tmp = h->A[a];
    h->A[a] = h->A[b];
    h->A[b] = tmp;
}

Heap HEAPinit(int maxN) {
    Heap h;
    h = (Heap) malloc(sizeof(*h));
    h->A = (Item *) malloc(maxN * sizeof(Item));
    h->heapsize = 0;

    return h;
}

void HEAPfree(Heap h) {
    free(h->A);
    free(h);
}

/**
 * * Insert Item at the end of the array
 *  ! no respect of heap properties
 */
void HEAPfill(Item x, Heap h) {
    h->A[h->heapsize++] = x;
}

/**
 * * Transform to heap a tree where the childs are heaps
 *  ? O(logN)
 */
void HEAPify(Heap h, int n) {
    int largest;

    if (LEFT(n) < h->heapsize &&
        KEYcmp(ITEMgetKEY(h->A[LEFT(n)]), ITEMgetKEY(h->A[n])) > 0)
        largest = LEFT(n);
    else 
        largest = n;

    if (RIGHT(n) < h->heapsize &&
        KEYcmp(ITEMgetKEY(h->A[RIGHT(n)]), ITEMgetKEY(h->A[largest])) > 0)
        largest = RIGHT(n);
    
    if (largest != n) {
        Swap(h, largest, n);
        HEAPify(h, largest);
    }
}

/**
 * * Convert a general array to heap
 * ? O(N)
 */
void HEAPbuild(Heap h) {
    int i;
    for (i = PARENT(h->heapsize - 1); i >= 0; i--)
        HEAPify(h, i);
}

/**
 * * Array sort using heap data structure
 * ? O(NlogN)
 */
void HEAPsort(Heap h) {
    int i, j;
    HEAPbuild(h);
    j = h->heapsize;

    for (i = h->heapsize-1; i > 0; i--) {
        Swap(h, 0, i);
        h->heapsize--;
        HEAPify(h, 0);
    }
    h->heapsize = j;
}