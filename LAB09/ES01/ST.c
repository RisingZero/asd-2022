#include "ST.h"

#define MAXC 31

typedef struct record {
    int id;
    char label[MAXC];
} Record;

struct st { 
    Record *records;
    int N; int maxN;
};

ST STinit(int N) {
    ST st = (ST) malloc(sizeof(*st));
    st->records = (Record *) malloc(N * sizeof(Record));
    st->maxN = N;
    st->N = 0;

    return st;
}

void STfree(ST st) {
    free(st->records);
    free(st);
}

static Record NEW(int id, char *label) {
    Record rec;
    rec.id = id;
    strcpy(rec.label, label);

    return rec;
}

void STinsert(ST st, char *label, int id) {
    Record rec = NEW(id, label);
    st->records[st->N++] = rec;
}

int STsearch(ST st, char *label) {
    int i;

    for (i = 0; i < st->N; i++) {
        if (strcmp(label, st->records[i].label) == 0)
            return i;
    }

    return -1;
}

char *STsearchByIndex(ST st, int index) {
    return st->records[index].label;
}

int STcount(ST st) {
    return st->N;
}