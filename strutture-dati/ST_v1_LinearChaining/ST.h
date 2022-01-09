#ifndef ST_MODULE
#define ST_MODULE

#include "Item.h"

typedef struct symboltable *ST;

ST STinit(int maxN, float r);
void STfree(ST st);

void STinsert(ST st, Item val);
Item STsearch(ST st, Key key);
void STdelete(ST st, Key key);

void STdisplay(ST st);

int STcount(ST st);
int STempty(ST st);

#endif