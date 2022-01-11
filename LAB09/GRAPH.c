#include "GRAPH.h"
#include "ST.h"

#define MAXC 31

typedef struct node *link;
struct node { int v; int wt; link next; };

struct graph {
    int V; int E;
    link *ladj;  // Lista delle adiacenze
    ST tab;
    link z; // Nodo sentinella
};

static link NEW(int v, int wt, link next) {
    link t = (link) malloc(sizeof(*t));

    t->next = next;
    t->v = v;
    t->wt = wt;

    return t;
}

static Edge EDGEcreate(int v, int w, int wt) {
    Edge e;
    e.v = v; e.w = w; e.wt = wt;
    return e;
}

Graph GRAPHinit(int V) {
    int i;

    Graph G = (Graph) malloc(sizeof(*G));
    G->V = V;
    G->E = 0;
    G->tab = STinit(V);
    G->z = NEW(-1, -1, NULL);
    G->ladj = (link *) malloc(V * sizeof(link));
    for (i = 0; i < V; i++) {
        G->ladj[i] = G->z;
    }

    return G;
}

void GRAPHfree(Graph G) {
    int i;
    link t, next;

    for (i = 0; i < G->V; i++) {
        for (t = G->ladj[i]; t != G->z; t = next) {
            next = t->next;
            free(t);
        }
    }
    STfree(G->tab);
    free(G->ladj);
    free(G->z);
    free(G);
}

Graph GRAPHload(FILE *fin) {
    int V, i, id1, id2, wt;
    char label1[MAXC], label2[MAXC];
    Graph G;

    fscanf(fin, "%d", &V);
    G = GRAPHinit(V);
    for (i = 0; i < V; i++) {
        fscanf(fin, "%s", label1);
        STinsert(G->tab, label1, i);
    }
    while (fscanf(fin, "%s %s %d", label1, label2, &wt) == 3) {
        id1 = STsearch(G->tab, label1);
        id2 = STsearch(G->tab, label2);
        if (id1 >= 0 && id2 >= 0) {
            GRAPHinsertE(G, id1, id2, wt);
        }
    }

    return G;
}

void GRAPHstore(Graph G, FILE *fout) {
    int i;
    Edge *a;

    a = malloc(G->E * sizeof(Edge));
    GRAPHedges(G, a);

    fprintf(fout, "%d\n", G->V);
    for (i = 0; i < G->V; i++) {
        fprintf(fout, "%s\n", STsearchByIndex(G->tab, i));
    }
    for (i = 0; i < G->E; i++) {
        fprintf(fout, "%s %s %d\n",
            STsearchByIndex(G->tab, a[i].v),
            STsearchByIndex(G->tab, a[i].w),
            a[i].wt
        );
    }
}

int GRAPHgetIndex(Graph G, char *label) {
    int id;

    id = STsearch(G->tab, label);
    if (id == -1) {
        id = STcount(G->tab);
        STinsert(G->tab, label, id);
    }
    return id;
}

static void insertE(Graph G, Edge edge) {
    G->ladj[edge.v] = NEW(edge.w, edge.wt, G->ladj[edge.v]);
    G->E++;
}

static void removeE(Graph G, Edge edge) {
    int v = edge.v, w = edge.w; link x;

    if (G->ladj[v]->v == w) {
        G->ladj[v] = G->ladj[v]->next;
        G->E--;
    }
    else {
        for (x = G->ladj[v]; x != G->z; x = x->next) {  
            if (x->next->v == w) {
                x->next = x->next->next;
                G->E--;
            }
        }
    }   
}

void GRAPHinsertE(Graph G, int id1, int id2, int wt) {
    insertE(G, EDGEcreate(id1, id2, wt));
}

void GRAPHremoveE(Graph G, int id1, int id2, int wt) {
    removeE(G, EDGEcreate(id1, id2, 0));
}

void GRAPHedges(Graph G, Edge *a) {
    int v, E = 0;
    link t;

    for (v = 0; v < G->V; v++) {
        for (t = G->ladj[v]; t != G->z; t = t->next) {
            a[E++] = EDGEcreate(v, t->v, t->wt);
        }
    }
}

void GRAPHfindESubsetToDAG(Graph G) {
    
}