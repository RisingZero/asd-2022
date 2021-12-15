#include "pgList.h"

typedef struct node_s *link;
struct node_s {
    pg_t pg;
    link next;
};

struct pgList_s {
    link headPg;
    link tailPg;
    int nPg;
};

pgList_t pgList_init() {
    pgList_t t = (pgList_t) malloc(sizeof(*t));
    t->headPg = t->tailPg = NULL;
    t->nPg = 0;
}

void pgList_free(pgList_t pgList) {
    link p = pgList->headPg, t = p->next;

    for ( ; t != NULL; p = t, t = t->next) {
        pg_clean(&(p->pg));
        free(p);
    }
    free(pgList);
}

void pgList_read(FILE *fp, pgList_t pgList) {
    pg_t pg;
    int N;

    N = 0;
    while (!feof(fp) && (pg_read(fp, &pg))) {
        pgList_insert(pgList, pg);
    }
}

void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray) {
    link t = pgList->headPg;

    pg_header(fp);
    while(t != NULL) {
        pg_print(fp, &(t->pg), invArray);
        t = t->next;
    }
}

void pgList_insert(pgList_t pgList, pg_t pg) {
    link node = (link) malloc(sizeof(*node));

    node->pg = pg;
    node->next = pgList->headPg; // insert at head
    pgList->headPg = node;
    if (pgList->nPg == 0)
        pgList->tailPg = pgList->headPg;

    pgList->nPg++;
}

void pgList_remove(pgList_t pgList, char* cod) {
    link t, p;
    pg_t *pgp;
    int found = 0;

    pgp = pgList_searchByCode(pgList, cod);

    // check head
    if (&(pgList->headPg->pg) == pgp) {
        t = pgList->headPg;
        pgList->headPg = t->next;
        pg_clean(&(t->pg));
        free(t);
        found = 1;
    }

    // check middle
    for (p = pgList->headPg, t = p->next; t != NULL && !found; p = t, t = t->next) {
        if (&(t->pg) == pgp) {
            p->next = t->next;
            if (t == pgList->tailPg)
                pgList->tailPg = p;
            pg_clean(&(t->pg));
            free(t);
            found = 1;
        }
    }

    // update dim if found
    if (found)
        pgList->nPg--;
}

pg_t *pgList_searchByCode(pgList_t pgList, char* cod) {
    link t = pgList->headPg;

    for ( ; t != NULL; t = t->next) {
        if (pg_compare_code(t->pg, cod) == 0) {
            return &(t->pg);
        }
    }

    return NULL;
}