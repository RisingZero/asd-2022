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

static int LEFT(int i) {
    return 2*i + 1; 
}

static int RIGHT(int i) {
    return 2*i + 2;
}

static int PARENT(int i) {
    return (i-1)/2;
}

static void Swap(PQ pq, int a, int b) {
    Item tmp = pq->A[a];
    pq->A[a] = pq->A[b];
    pq->A[b] = tmp;
}

static void HEAPify(PQ pq, int i) {
    int largest, l, r;
    l = LEFT(i); r = RIGHT(i);

    if (l <= pq->heapsize &&
    PRIORITYcmp(ITEMgetPriority(pq->A[l]), ITEMgetPriority(pq->A[i])) > 0) {
        largest = l;
    } else
        largest = i;

    if (r <= pq->heapsize &&
    PRIORITYcmp(ITEMgetPriority(pq->A[r]), ITEMgetPriority(pq->A[largest])) > 0) {
        largest = l;
    }

    if (largest != i) {
        Swap(pq, largest, i);
        HEAPify(pq, largest);
    }

    return;
}

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

int PQempty(PQ pq) {
    return pq->heapsize == 0;
}

int PQsize(PQ pq) {
    return pq->heapsize;
}

Item PQshowMax(PQ pq) {
    return pq->A[0];
}

Item PQextractMax(PQ pq) {
    Item x;
    Swap(pq, 0, pq->heapsize-1);
    x = pq->A[pq->heapsize-1];
    pq->heapsize--;
    HEAPify(pq, 0);

    return x;
}

void PQinsert(PQ pq, Item x) {
    int i;
    i = pq->heapsize++;
    while (i >= 1  &&
    PRIORITYcmp(ITEMgetPriority(pq->A[PARENT(i)]), ITEMgetPriority(x)) < 0) {
        pq->A[i] = pq->A[PARENT(i)];
        i = PARENT(i);
    }
    pq->A[i] = x;
    return;
}

/**
 * * Updates the priority of the item passed as parameter (must be already present in the pq)
 * ? O(N)
 */
void PQchange(PQ pq, Item x) {
    int found = 0, i, pos;

    // Linear search in the array O(N)
    for(i = 0; i < pq->heapsize && !found; i++) {
        if (KEYcmp(ITEMgetKEY1(x), ITEMgetKEY1(pq->A[i])) == 0) {
            pos = i;
            found = 1;
        }
    }

    // Logaritmic repositioning with insert logic or heapify logic O(logN)
    if (found) {
        while (pos >= 1 &&
        PRIORITYcmp(ITEMgetPriority(x), ITEMgetPriority(pq->A[PARENT(pos)])) > 0) {
            pq->A[pos] = pq->A[PARENT(pos)];
            pos = PARENT(pos);
        }
        pq->A[pos] = x;
        HEAPify(pq, pos);
    } else
        printf("Key not found!\n");
    
    return;
}