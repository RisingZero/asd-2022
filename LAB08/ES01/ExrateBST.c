#include "ExrateBST.h"

#define S 3

typedef struct node_ *link;
struct node_ {
    Exrate exrate;
    link left, right;
};

struct exrate_bst_ {
    link root;
    link z;
    int N;
};

static link NEW(Exrate data, link l, link r) {
    link x = (link) malloc(sizeof(*x));
    x->left = l; x->right = r; x->exrate = data;
    return x;
}

static link rotR(link h) {
    link t = h->left;
    h->left = t->right;
    t->right = h;
    return t;
}

static link rotL(link h) {
    link t = h->right;
    h->right = t->left;
    t->left = h;
    return t;
}

ExrateBST ExrateBST_init() {
    ExrateBST bst = (ExrateBST) malloc(sizeof(*bst));
    bst->root = bst->z = NEW(Exrate_null(), NULL, NULL);
    bst->N = 0;
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
        return z;

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
    link *tp = &(bst->root);

    if (bst->root == bst->z) {
        bst->root = NEW(exrate, bst->z, bst->z);
        bst->N++;
        return;
    }

    while (*tp != bst->z) {
        if (compareDate((*tp)->exrate.date, exrate.date) > 0)
            tp = &((*tp)->left);
        else
            tp = &((*tp)->right);
    }
    *tp = NEW(exrate, bst->z, bst->z);
    bst->N++;
}

void ExrateBST_insert(ExrateBST bst, Exrate exrate) {
    link tp;
    int tempQ;

    if ((tp = ExrateBST_getByDate(bst, exrate.date)) != bst->z) {
        tempQ  = ((tp->exrate.n * tp->exrate.q) + (exrate.n * exrate.q))/(tp->exrate.n + exrate.n);
        tp->exrate.q = tempQ;
        tp->exrate.n += exrate.n;
    } else {
        ExrateBST_insertLeaf(bst, exrate);
    }
}

static void mergeR(ExrateBST dest, link r, link z) {
    if (r == z)
        return;

    mergeR(dest, r->left, z);
    mergeR(dest, r->right, z);
    ExrateBST_insert(dest, r->exrate);
}

void ExrateBST_merge(ExrateBST dest, ExrateBST src) {
    mergeR(dest, src->root, src->z);
}

static void recursiveLength(link t, link z, int *min, int *max, int *count) {
    if (t == z) {
        if (*count < *min) *min = *count;
        if (*count > *max) *max = *count;
        return;
    }

    (*count)++;
    recursiveLength(t->left, z, min, max, count);
    recursiveLength(t->right, z, min, max, count);
    (*count)--;
    return;
}

static int ExrateBST_calculateMinMaxLengthDiff(ExrateBST bst) {
    int min = bst->N, max = 0;
    int count = 0;

    recursiveLength(bst->root, bst->z, &min, &max, &count);
    return max-min;
}

static int visitInOrderWithIdx(link t, int index, int *count, link z, link *node) {
    if (t->left != z)
        visitInOrderWithIdx(t->left, index, count, z, node);
    if (*count == index) {
        *node = t;
    }
    (*count)++;
    if (t->right != z)
        visitInOrderWithIdx(t->right, index, count, z, node);
}

static link ExrateBST_findMedianNode(ExrateBST bst, int index) {
    link t = bst->root, z = bst->z, node;
    int count = 0;

    visitInOrderWithIdx(t, index, &count, z, &node);

    return node;
}

static link partR(link t, link ref) {
    if (compareDate(t->exrate.date, ref->exrate.date) > 0) {
        t->left = partR(t->left, ref);
        t = rotR(t);
    } else if (compareDate(t->exrate.date, ref->exrate.date) < 0) {
        t->right = partR(t->right, ref);
        t = rotL(t);
    }
    return t;
}

void ExrateBST_balance(ExrateBST bst) {
    int diff, idx;
    link medianNode;

    // calculate the difference between the max and the min root-leaf distances
    diff = ExrateBST_calculateMinMaxLengthDiff(bst);

    if (diff >= S) {
        printf("Found unbalance of %d, rebalancing...\n", diff);
        // find median value
        idx = (bst->N + 1)/2;
        medianNode = ExrateBST_findMedianNode(bst, idx);
        printf("median: %d\n", idx);
        // partition on median value
        bst->root = partR(bst->root, medianNode);

        diff = ExrateBST_calculateMinMaxLengthDiff(bst);
        printf("Rebalanced! New length difference: %d\n", diff);
    } else {
        printf("No significative unbalance found\n");
    }
}

Exrate ExrateBST_search(ExrateBST bst, Datetime date) {
    link t = ExrateBST_getByDate(bst, date);
    if (t != bst->z)
        return t->exrate;
    else
        return Exrate_null();
}

static void visitInOrderCheckInterval(link h, Datetime date1, Datetime date2, int withInterval, link z, Exrate *min, Exrate *max) {
    if (h == z) return;

    visitInOrderCheckInterval(h->left, date1, date2, withInterval, z, min, max);
    if ((withInterval && compareDate(h->exrate.date, date1) >= 0 && compareDate(h->exrate.date, date2) <= 0) || !withInterval) {
        if (h->exrate.q > max->q)
            *max = h->exrate;
        if (h->exrate.q < min->q)
            *min = h->exrate;
    }
    visitInOrderCheckInterval(h->right, date1, date2, withInterval, z, min, max);
}

void ExrateBST_minMaxInInterval(ExrateBST bst, Datetime date1, Datetime date2, int withInterval) {
    Exrate min, max;
    min = max = bst->root->exrate;
    visitInOrderCheckInterval(bst->root, date1, date2, withInterval, bst->z, &min, &max);
    printf("Min quotation: ");
    Exrate_display(stdout, min); 
    printf("Max quotation: ");
    Exrate_display(stdout, max);
}
