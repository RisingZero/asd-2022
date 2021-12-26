#include "ST.h"
#include "Item.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct STnode *link;
struct STnode {
    Item item;
    link next;
};

struct symboltable {
    link *heads;
    int N;
    int M;
    link z;
};

// Hash of a string represented as a polynomial (ASCII base) and the Horner representation
static int stringHash(Key k, int M) {
    int h = 0, base = 127;

    for ( ; *k != '\0'; k++)
        h = (base * h + *k) % M;

    return h;
}

// Hash of a string with changing base -> better distribution of calculated hashes
static int stringHashU(Key k, int M) {
    int h, a = 31415, b = 27183;

    for (h = 0; *k != '\0'; k++, a = a*b % (M-1))
        h = (a * h + *k) % M;

    return h;
}

static link NEW(Item item, link next) {
    link x = (link) malloc(sizeof(*x));

    x->item = item;
    x->next = next;

    return x;
}

static int STsizeSet(int maxN, float r) {
    int i, size;
    int primes[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};

    size = maxN/r;
    if (size < primes[15]) {
        for (i = 0; i < 16; i++) {
            if (size < primes[i])
                return primes[i];
        }
    } else {
        printf("Too many entries!\n");
        return -1;
    }
}

ST STinit(int maxN, float r) {
    int i;
    ST st;

    st = (ST) malloc(sizeof(*st));
    st->N = 0;
    st->M = STsizeSet(maxN, r);
    st->heads = (link*) malloc(st->M * sizeof(link));
    st->z = NEW(ITEMsetNull(), NULL);

    for (i = 0; i < st->M; i++)
        st->heads[i] = st->z;
    return st;
}

void STfree(ST st) {
    int i;
    link t, u;

    for (i = 0; i < st->M; i++) {
        for (t = st->heads[i], u = t->next; t != st->z; t = u, u = t->next)
            free(t);
    }
    free(st->z);
    free(st->heads);
    free(st);
}

int STcount(ST st) {
    return st->N;
}

int STempty(ST st) {
    return st->N == 0;
}

void STinsert(ST st, Item val) {
    int i;
    i = stringHash(ITEMgetKEY(val), st->M);
    st->heads[i] = NEW(val, st->heads[i]);
}

static Item searchR(link head, Key key, link z) {
    if (head == z)
        return ITEMsetNull();

    if (KEYcmp(ITEMgetKEY(head->item), key) == 0)
        return head->item;

    return searchR(head->next, key, z);
}

Item STsearch(ST st, Key key) {
    return searchR(st->heads[stringHash(key, st->M)], key, st->z);
}

static link deleteR(link t, Key key, link z) {
    link x;
    if (t == z) return z;
    if (KEYcmp(ITEMgetKEY(t->item), key) == 0) {
        x = t->next; free(t); return x;
    }
    t->next = deleteR(t->next, key, z);
    return t;
}

void STdelete(ST st, Key key) {
    int i = stringHash(key, st->M);
    st->heads[i] = deleteR(st->heads[i], key, st->z);
}
