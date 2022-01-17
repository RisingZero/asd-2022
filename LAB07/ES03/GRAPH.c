#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "GRAPH.h"

typedef struct node *link;
struct node { 
    int id;
    int wt;
    link next;
};

struct graph {
    int V; int E;
    int **madj;
    link *ladj;
    link z;
    ST tab;
};

static link NEW(int id, int wt, link next) {
    link t = (link) malloc(sizeof(*t));
    t->id = id; t->wt = wt; t->next = next;
    return t;
}

Graph GRAPHinit(int V) {
    Graph G = (Graph) malloc(sizeof(*G));
    int i;


    G->V = V;
    G->E = 0;

    G->madj = (int **) malloc(V * sizeof(int*));
    for (i = 0; i < V; i++) {
        G->madj[i] = (int *) calloc(V, sizeof(int));
    }

    G->tab = STinit(V);

    G->z = NEW(-1, -1, NULL);
    G->ladj = (link *) malloc(V * sizeof(link));
    for (i = 0; i < V; i++)
        G->ladj[i] = G->z;

    return G;
}

static void freeRList(link t, link z) {
    if (t == z) return;

    freeRList(t->next, z);
    free(t);
}

void GRAPHfree(Graph G) {
    int i;
    link t, x;

    STfree(G->tab);
    for (i = 0; i < G->V; i++) {
        free(G->madj[i]);
        freeRList(G->ladj[i], G->z);
    }
    free(G->z);
    free(G->ladj);
    free(G->madj);
    free(G);
}

void GRAPHinsertE(Graph G, char *name1, char *subnet1, char *name2, char *subnet2, int wt) {
    int id1, id2;
    Item item1, item2;

    item1 = ITEMcreate(name1, subnet1);
    item2 = ITEMcreate(name2, subnet2);

    id1 = STinsert(G->tab, item1);
    id2 = STinsert(G->tab, item2);

    G->madj[id1][id2] = wt;
    G->madj[id2][id1] = wt;

    G->E++;
}

void GRAPHbuildLadj(Graph G) {
    int i, j;
    link t;

    for (i = 0; i < G->V; i++) {
        freeRList(G->ladj[i], G->z);

    }

    for (i = 0; i < G->V; i++) {
        for (j = 0; j < G->V; j++) {
            if (G->madj[i][j] != 0)
                G->ladj[i] = NEW(j, G->madj[i][j], G->ladj[i]);
        }
    }

    for (i = 0; i < G->V; i++) {
        if (G->ladj[i] != G->z) {
            printf("%d", i);
            for (t = G->ladj[i]; t != G->z; t = t->next) {
                printf(" -> (%d,%d)", t->id, t->wt);
            }
            printf("\n");
        }
    }
}

static int vertexCompare(void *G, const void *a, const void *b) {
    link a1 = *(link *)a;
    link b1 = *(link *)b;
    ST st = (*(Graph *)G)->tab;

    return (strcmp(STsearchByIndex(st, a1->id).name, STsearchByIndex(st, b1->id).name));
}

static void EDGEsort(link *v, int N, Graph G) {
    qsort_s(v, N, sizeof(link), vertexCompare, &G);
}

void GRAPHdisplayVertexEdgesOrdered(Graph G) {
    Item *items;
    link *edges;
    int i, j, cnt, V;

    V = STcount(G->tab);

    items = (Item *) malloc(V * sizeof(Item));
    edges = (link *) malloc(G->V * sizeof(link));

    for (i = 0; i < V; i++) {
        items[i] = STsearchByIndex(G->tab, i);
    }

    ITEMsort(items, V);

    for (i = 0; i < V; i++) {
        // show vertex
        printf("%s %s:\n", items[i].name, items[i].subnet);

        cnt = 0;
        for (j = 0; j < G->V; j++) {
            if (G->madj[i][j] != 0)
                edges[cnt++] = NEW(j, G->madj[i][j], G->z);
        }

        EDGEsort(edges, cnt, G);

        // show edges
        for (j = 0; j < cnt; j++) {
            // show edge
            printf("\t-> %s %s\n", STsearchByIndex(G->tab, edges[j]->id).name, STsearchByIndex(G->tab, edges[j]->id).subnet);
        }

        for (j = 0; j < cnt; j++) 
            free(edges[j]);
    }

    free(edges);
    free(items);
}

static int check3AdjacencyM(Graph G, int id1, int id2, int id3) {
    int res = 1;

    // check 1 -> 2,3
    if (G->madj[id1][id2] == 0 || G->madj[id1][id3] == 0)
        res = 0;

    // check 2 -> 1,3
    if (G->madj[id2][id1] == 0 || G->madj[id2][id3] == 0)
        res = 0;

    // check 3 -> 1,2
    if (G->madj[id3][id1] == 0 || G->madj[id3][id2] == 0)
        res = 0;

    return res;

}

static int listSearch(link h, int id, link z) {
    if (h == z) return 0;

    if (h->id == id) return 1;
    return listSearch(h->next, id, z);
}

static int check3AdjacencyL(Graph G, int id1, int id2, int id3) {
    int res = 1;

    // check 1 -> 2,3
    if (!listSearch(G->ladj[id1], id2, G->z) || !listSearch(G->ladj[id1], id3, G->z))
        res = 0;

    // check 2 -> 1,3
    if (!listSearch(G->ladj[id2], id1, G->z) || !listSearch(G->ladj[id2], id3, G->z))
        res = 0;

    // check 3 -> 1,2
    if (!listSearch(G->ladj[id3], id1, G->z) || !listSearch(G->ladj[id3], id2, G->z))
        res = 0;

    return res;

}

void GRAPHthreeVAdjacency(Graph G) {
    Item item1, item2, item3;
    int id1, id2, id3;

    int areAdj;

    item1 = ITEMread();
    item2 = ITEMread();
    item3 = ITEMread();

    id1 = STsearch(G->tab, item1);
    id2 = STsearch(G->tab, item2);
    id3 = STsearch(G->tab, item3);

    if (id1 == -1 || id2 == -1 || id3 == -1) {
        printf("ERROR, uno dei vertici indicati non esiste\n");
        return;
    }

    areAdj = check3AdjacencyM(G, id1, id2, id3);

    if (areAdj)
        printf("Sono adiacenti a due a due\n");
    else
        printf("Non sono adiacenti a due a due\n");
}
