#include "PQ.h"

/**
 * * Priority Queue implemented with HEAP data structure
 * * v2 --> No Item.h dependency, in memory only index inside the item array and the priority (PQchange O(logN))
 * ? Insert     O(logN)
 * ? ShowMax    O(1)
 * ? ExtractMax O(logN)
 */

typedef struct heap_item_s {
    int index;
    int prio;
} heapItem;

struct pqueue {
    heapItem *A;
    int heapsize;
    int *qp; // Vector with links between item and position in the heap
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
    heapItem tmp = pq->A[a];
    int tmpi = pq->qp[pq->A[a].index];
    pq->A[a] = pq->A[b];
    pq->qp[pq->A[a].index] = pq->qp[pq->A[b].index];
    pq->A[b] = tmp;
    pq->qp[pq->A[b].index] = tmpi;
}

static void HEAPify(PQ pq, int i) {
    int largest, l, r;
    l = LEFT(i); r = RIGHT(i);

    if (l <= pq->heapsize &&
    pq->A[l].prio > pq->A[i].prio) {
        largest = l;
    } else
        largest = i;

    if (r <= pq->heapsize &&
    pq->A[r].prio > pq->A[largest].prio) {
        largest = l;
    }

    if (largest != i) {
        Swap(pq, largest, i);
        HEAPify(pq, largest);
    }

    return;
}

PQ PQ_init(int maxN) {
    int i;
    PQ pq;
    pq = (PQ) malloc(sizeof(*pq));
    pq->A = (heapItem *) malloc(maxN * sizeof(heapItem));
    for (i = 0; i < maxN; i++) {
        pq->A[i].index = -1; pq->qp[i] = -1;
    }
    pq->heapsize = 0;

    return pq;
}

void PQ_free(PQ pq) {
    free(pq->A);
    free(pq->qp);
    free(pq);
}

int PQempty(PQ pq) {
    return pq->heapsize == 0;
}

int PQsize(PQ pq) {
    return pq->heapsize;
}

int PQshowMax(PQ pq) {
    return pq->A[0].index;
}

int PQextractMax(PQ pq) {
    heapItem x;
    Swap(pq, 0, pq->heapsize-1);
    x = pq->A[pq->heapsize-1];
    pq->qp[x.index] = -1;
    pq->heapsize--;
    HEAPify(pq, 0);

    return x.index;
}

void PQinsert(PQ pq, int index, int prio) {
    int i;
    i = pq->heapsize++;
    while (i >= 1  &&
    pq->A[PARENT(i)].prio < prio) {
        pq->A[i] = pq->A[PARENT(i)];
        pq->qp[pq->A[i].index] = i;
        i = PARENT(i);
    }
    pq->A[i].index = index;
    pq->A[i].prio = prio;
    pq->qp[index] = i;
    return;
}

/**
 * * Updates the priority of the item passed as parameter (must be already present in the pq)
 * ? O(N)
 */
void PQchange(PQ pq, int index, int prio) {
    int found = 0, i, pos;
    heapItem tmp;

    tmp.index = index;
    tmp.prio = prio;
    pos = pq->qp[index];

    while (pos >= 1 &&
    prio > pq->A[PARENT(pos)].prio) {
        pq->A[pos] = pq->A[PARENT(pos)];
        pq->qp[pq->A[pos].index] = pos;
        pos = PARENT(pos);
    }
    pq->A[pos] = tmp;
    pq->qp[index] = pos;
    HEAPify(pq, pos);
    
    return;
}