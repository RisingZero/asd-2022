#include "GRAPH.h"
#include "ST.h"

#define MAXC 100

struct graph {
    int V; int E;
    int **madj; // Matrice di adiacenza
    ST tab;
};

static int **MATRIXinit(int r, int c, int val) {
    int i, j;
    int **t = (int **) malloc(r * sizeof(int*));
    for (i = 0; i < r; i++) {
        t[i] = (int *) malloc(c * sizeof(int));
        for (j = 0; j < c; j++) t[i][j] = val;
    }
    return t;
}

static void MATRIXfree(int **mat, int r, int c) {
    int i;
    for (i = 0; i < r; i++) {
        free(mat[i]);
    }
    free(mat);
}

static Edge EDGEcreate(int v, int w, int wt) {
    Edge e;
    e.v = v; e.w = w; e.wt = wt;
    return e;
}

Graph GRAPHinit(int V) {
    Graph G = (Graph) malloc(sizeof(*G));
    G->V = V;
    G->E = 0;
    G->tab = STinit(V);
    G->madj = MATRIXinit(V, V, 0);

    return G;
}

void GRAPHfree(Graph G) {
    STfree(G->tab);
    MATRIXfree(G->madj, G->V, G->V);
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
    if ((G->madj)[edge.v][edge.w] == 0)
        G->E++;
    (G->madj)[edge.v][edge.w] = edge.wt;
}

static void removeE(Graph G, Edge edge) {
    if ((G->madj)[edge.v][edge.w] != 0)
        G->E--;
    (G->madj)[edge.v][edge.w] = edge.wt;
}

void GRAPHinsertE(Graph G, int id1, int id2, int wt) {
    insertE(G, EDGEcreate(id1, id2, wt));
}

void GRAPHremoveE(Graph G, int id1, int id2, int wt) {
    removeE(G, EDGEcreate(id1, id2, 0));
}

void GRAPHedges(Graph G, Edge *a) {
    int v, w, E = 0;

    for (v = 0; v < G->V; v++) {
        for (w = 0; w < G->V; w++) {
            if (G->madj[v][w] != 0)
                a[E++] = EDGEcreate(v, w, G->madj[v][w]);
        }
    }
    return;
}

static int pathR(Graph G, int v, int w, int *visited) {
    int t;

    if (v == w)
        return 1;

    visited[v] = 1;
    for (t = 0; t < G->V; t++) {
        if (G->madj[v][t] != 0) {
            if (visited[t] == 0) {
                if (pathR(G, t, w, visited)) {
                    printf("(%s, %s) in path\n", STsearch(G->tab, v), STsearch(G->tab, t));
                    return 1;
                }
            }
        }
    }
    return 0;
}

void GRAPHpath(Graph G, int id1, int id2) {
    int t, found, *visited;

    visited = (int *) malloc(G->V * sizeof(int));
    for (t = 0; t < G->V; t++)
        visited[t] = 0;
    
    if (id1 < 0 || id2 < 0)
        return;

    found = pathR(G, id1, id2, visited);
    if (found == 0)
        printf("\nPath not found!\n");
}

static int pathHR(Graph G, int v, int w, int d, int *visited) {  // d is the remaining length for a V-1 length path
    int t;

    if (v == w) {
        if (d == 0)
            return 1;
        else
            return 0;
    }

    visited[v] = 1;
    for (t = 0; t < G->V; t++) {
        if (G->madj[v][t] != 0 && visited[t] == 0) {
            if (pathRH(G, t, w, d-1, visited)) {
                printf("(%s, %s) in path\n", STsearch(G->tab, v), STsearch(G->tab, t));
                return 1;
            }
        }
    }
    visited[v] = 0;
    return 0;
}

void GRAPHpathH(Graph G, int id1, int id2) {
    int t, found, *visited;

    visited = (int *) malloc(G->V * sizeof(int));
    for (t = 0; t < G->V; t++)
        visited[t] = 0;
    
    if (id1 < 0 || id2 < 0)
        return;

    found = pathHR(G, id1, id2, G->V-1, visited);
    if (found == 0)
        printf("\nPath not found!\n");
}

void GRAPHbfs(Graph G, int id);
void GRAPHdfs(Graph G, int id);
int GRAPHcc(Graph G);
int GRAPHscc(Graph G);