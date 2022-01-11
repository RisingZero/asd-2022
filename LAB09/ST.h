#ifndef ST_ADT_MODULE
#define ST_ADT_MODULE

#include <string.h>
#include <stdlib.h>

typedef struct st *ST;

ST STinit(int N);
void STfree(ST st);

void STinsert(ST st, char *label, int id);

int STsearch(ST st, char *label);
char *STsearchByIndex(ST st, int index);

int STcount(ST st);

#endif