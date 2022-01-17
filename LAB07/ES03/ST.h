#ifndef ST_MODULE
#define ST_MODULE

#include "Item.h"

typedef struct st *ST;

ST STinit(int N);
void STfree(ST st);

int STinsert(ST st, Item item);

Item STsearchByIndex(ST st, int index);
int STsearch(ST st, Item item);

int STcount(ST st);

#endif