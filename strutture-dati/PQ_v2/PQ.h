#ifndef PQ_MODULE
#define PQ_MOODULE

#include <stdlib.h>

typedef struct pqueue *PQ;

PQ PQ_init(int maxN);
void PG_free(PQ pq);

int PQempty(PQ pq);
int PQsize(PQ pq);

int PQshowMax(PQ pq);
int PQextractMax(PQ pq);

void PQinsert(PQ pq, int index, int prio);

void PQchange(PQ pq, int index, int prio);

#endif