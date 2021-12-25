#ifndef PQ_MODULE
#define PQ_MOODULE

#include <stdlib.h>

#include "Item.h"

typedef struct pqueue *PQ;

PQ PQ_init(int maxN);
void PG_free(PQ pq);

int PQempty(PQ pq);
int PQsize(PQ pq);

Item PQshowMax(PQ pq);
Item PQextractMax(PQ pq);

void PQinsert(PQ pq, Item x);

#endif