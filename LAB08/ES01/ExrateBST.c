#include "ExrateBST.h"

typedef struct node_ *link;
struct node_ {
    Exrate exrate;
    link left, right;
};

struct exrate_bst_ {
    link root;
    link z;
};

static link NEW(Exrate data, link l, link r) {
    link x = (link) malloc(sizeof(*x));
    x->left = l; x->right = r; x->exrate = data;
    return x;
}

ExrateBST ExrateBST_init() {
    ExrateBST bst = (ExrateBST) malloc(sizeof(*bst));
    bst->root = bst->z = NEW(Exrate_null(), NULL, NULL);
    return bst;
}

static void treeFree(link h, link z) {
    if (h == z) return;

    treeFree(h->left, z);
    treeFree(h->right, z);
    free(h);
}

void ExrateBST_free(ExrateBST bst) {
    if (bst == NULL) return;

    treeFree(bst->root, bst->z);
    free(bst->z);
    free(bst);
}

static link searchR(link h, Datetime date, link z) {
    int cmp;

    if (h == z)
        return NULL;

    cmp = compareDate(date, h->exrate.date);
    if (cmp == 0)
        return h;
    if (cmp > 0)
        return searchR(h->right, date, z);
    else
        return searchR(h->left, date, z);
    
}

static link ExrateBST_getByDate(ExrateBST bst, Datetime date) {
    return searchR(bst->root, date, bst->z);
}

static void ExrateBST_insertLeaf(ExrateBST bst, Exrate exrate) {
    link tp = bst->root;

    if (bst->root == bst->z) {
        bst->root = NEW(exrate, bst->z, bst->z);
    }

    while (tp != bst->z) {
        if (compareDate(tp->exrate.date, exrate.date) > 0)
            tp = tp->left;
        else
            tp = tp->right;
    }
    tp = NEW(exrate, bst->z, bst->z);
}

void ExrateBST_insert(ExrateBST bst, Exrate exrate) {
    link tp;
    int tempQ;

    if ((tp = ExrateBST_getByDate(bst, exrate.date)) != NULL) {
        tempQ  = ((tp->exrate.n * tp->exrate.q) + (exrate.n * exrate.q))/(tp->exrate.n + exrate.n);
        tp->exrate.q = tempQ;
        tp->exrate.n += exrate.n;
    } else {
        ExrateBST_insertLeaf(bst, exrate);
    }
}

void ExrateBST_balance(ExrateBST bst) {
    
}

