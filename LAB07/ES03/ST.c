#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ST.h"

struct st {
    Item *v;
    int N;
    int maxN;
};

ST STinit(int N) {
    ST st = (ST) malloc(sizeof(*st));

    st->v = (Item *) malloc(N * sizeof(Item));
    st->N = 0;
    st->maxN = N;

    return st;
}

void STfree(ST st) {
    free(st->v);
    free(st);
}

int STinsert(ST st, Item item) {
    int i;

    if (st->N == st->maxN) {
        printf("Error, max size of symbol table reached\n");
        return -1;
    }

    for (i = 0; i < st->N; i++) {
        if (strcmp(item.name, st->v[i].name) == 0)
            return i;
    }
    st->v[i] = item;
    st->N++;
    return i;
}

Item STsearchByIndex(ST st, int index) {
    if (index >= st->N)
        return ITEMnull();

    return st->v[index];
}

int STsearch(ST st, Item item) {
    int i;
    for (i = 0; i < st->N; i++) {
        if (strcmp(item.name, st->v[i].name) == 0)
            return i;
    }
    return -1;
}

int STcount(ST st) {
    return st->N;
}
