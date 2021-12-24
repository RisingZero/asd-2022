#include "PQ.h"

/**
 * * Priority Queue implemented with HEAP data structure
 * ? Insert     O(logN)
 * ? ShowMax    O(1)
 * ? ExtractMax O(logN)
 */
struct pqueue {
    Item *A;
    int heapsize; 
};

PQ PQ_init(int maxN) {
    PQ pq;
    pq = (PQ) malloc(sizeof(*pq));
    pq->A = (Item *) malloc(maxN * sizeof(Item));
    pq->heapsize = 0;

    return pq;
}

void PQ_free(PQ pq) {
    free(pq->A);
    free(pq);
}


