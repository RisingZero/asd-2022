#include "GRAPH.h"

#define MAXC 31
#define MINWT -32768

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

static int EDGEcompare(Edge a, Edge b) {
    return (a.wt == b.wt && a.v == b.v && a.w == b.w);
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

int GRAPHvertexCount(Graph G) {
    return G->V;
}

int GRAPHedgeCount(Graph G) {
    return G->E;
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
    int v = edge.v, w = edge.w, wt = edge.wt; link x;

    if (G->ladj[v]->v == w && G->ladj[v]->wt == wt) {
        G->ladj[v] = G->ladj[v]->next;
        G->E--;
    }
    else {
        for (x = G->ladj[v]; x != G->z; x = x->next) {  
            if (x->next->v == w && x->next->wt == wt) {
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

// Edge-Classify dfs -> backward edges classification through a dfs over the graph
static void ECdfsR(Graph G, Edge e, Edge *edges, int *pre, int *post, int *time, int *time1) {
    int v = e.v, w = e.w;
    link t;

    pre[w] = (*time)++;
    for (t = G->ladj[w]; t != G->z; t = t->next) {
        if (pre[t->v] == -1) {
            ECdfsR(G, EDGEcreate(w, t->v, t->wt), edges, pre, post, time, time1);
        } else {
            if (post[t->v] == -1) { // Backward edge
                edges[(*time1)++] = EDGEcreate(w, t->v, t->wt);
            }
        }
    }
    post[w] = (*time)++;
}

void GRAPHfindESubsetToDAG(Graph G, Edge *bestEdgeSet, int *bestSetCardinality) {
    Edge **edges;
    int *pre, *post, time, time1, i, j;
    int cnt, bestCnt = G->E, weightCnt, bestWeight = 0;
    int *bestEdgeSets, bestSet = -1;
    Edge e;

    // Inizialization of used data structures
    pre = (int *) malloc(G->V * sizeof(int));
    post = (int *) malloc(G->V * sizeof(int));
    edges = (Edge **) malloc(G->V * sizeof(Edge *));
    for (i = 0; i < G->V; i++) {
        edges[i] = (Edge *) malloc(G->E * sizeof(Edge));
        for (j = 0; j < G->E; j++) {
            edges[i][j] = EDGEcreate(0, 0, 0);
        }
    }

    // DFS starting from each of the vertices to find all possible classifications of Backward edges sets
    for (i = 0; i < G->V; i++) {
        time = 0; time1 = 0;
        for (j = 0; j < G->V; j++) {
            pre[j] = -1; post[j] = -1;
        }

        ECdfsR(G, EDGEcreate(i, i, 0), edges[i], pre, post, &time, &time1);
        for (j = 0; j < G->V; j++)
            if (pre[j] == -1)
                ECdfsR(G, EDGEcreate(j, j, 0), edges[i], pre, post, &time, &time1);
    }

    // Find minimum cardinality of backward edges sets
    bestEdgeSets = (int *) malloc(G->V * sizeof(int));
    for (i = 0; i < G->V; i++) {
        cnt = 0;
        for (j = 0; j < G->E; j++) {
            if (edges[i][j].wt != 0) {
                cnt++;
            }
        }
        if (cnt < bestCnt)
            bestCnt = cnt;

        if (cnt > bestCnt)
            bestEdgeSets[i] = 0;
        else
            bestEdgeSets[i] = 1;
    }

    *bestSetCardinality = bestCnt;

    if (bestCnt > 0) {
        printf("Minimum cardinality: %d\n", bestCnt);
        printf("Best edge sets:\n");
        for (i = 0; i < G->V; i++) {
            weightCnt = 0;
            if (bestEdgeSets[i] == 1) {
                printf("{");
                for (j = 0; j < G->E; j++) {
                    if (edges[i][j].wt != 0) {
                        weightCnt += edges[i][j].wt;
                        printf(" (%s, %s, wt: %d)", STsearchByIndex(G->tab,edges[i][j].v), STsearchByIndex(G->tab, edges[i][j].w), edges[i][j].wt);
                    }
                }
                printf(" }\n");
            }

            if (weightCnt > bestWeight) {
                bestWeight = weightCnt;
                bestSet = i;
            }
        }

        for (j = 0; j < G->E; j++) {
            bestEdgeSet[j] = EDGEcreate(0, 0, 0);
        }

        if (bestSet != -1) {
            printf("Best weight sum: %d\n", bestWeight);
            printf("Best edge set with best weight sum:\n");
            printf("{");
            for (j = 0; j < G->E; j++) {
                if (edges[bestSet][j].wt != 0)
                    printf(" (%s, %s, wt: %d)", STsearchByIndex(G->tab,edges[bestSet][j].v), STsearchByIndex(G->tab, edges[bestSet][j].w), edges[bestSet][j].wt);
                bestEdgeSet[j] = EDGEcreate(edges[bestSet][j].v, edges[bestSet][j].w, edges[bestSet][j].wt);
            }
            printf(" }\n");
            }
    }
    

    free(pre);
    free(post);
    for (i = 0; i < G->V; i++) {
        free(edges[i]);
    }
    free(edges);
    free(bestEdgeSets);
}

Graph GRAPHcreateFromGraphEdgeSubtraction(Graph G, Edge *toBeRemoved) {
    Edge *a;
    Graph G2 = GRAPHinit(G->V);
    int i, j, found;

    for (i = 0; i < G->V; i++) {
        STinsert(G2->tab, STsearchByIndex(G->tab, i), i);
    }

    a = (Edge *) malloc(G->E * sizeof(Edge));
    GRAPHedges(G, a);

    for (i = 0; i < G->E; i++) {
        found = 0;
        for (j = 0; j < G->E && !found; j++) {
            if (EDGEcompare(a[i], toBeRemoved[j])) {
                found = 1;
            }
        }

        if (!found) {
            GRAPHinsertE(G2, a[i].v, a[i].w, a[i].wt);
        }
    }

    return G2;
}

static int GRAPHcheckSource(Graph G, int id) {
    Edge *a;
    int i;

    a = (Edge *) malloc(G->E * sizeof(Edge));
    GRAPHedges(G, a);

    for (i = 0; i < G->E; i++) {
        if (a[i].w == id) {
            return 0;
        }
    }

    return 1;
}

void GRAPHfindMaxPathsFromSources(Graph G) {
    int *d, *st, i, j, k;
    link t;
    
    d = (int *) malloc(G->V * sizeof(int));
    st = (int *) malloc(G->V * sizeof(int));

    for (i = 0; i < G->V; i++) {
        if (GRAPHcheckSource(G, i)) {

            for (j = 0; j< G->V; j++) {
                d[j] = MINWT;
                st[j] = -1;
            }
            d[i] = 0; st[i] = i;

            for (j = 0; j < G->V-1; j++) {
                for (k = 0; k < G->V; k++) {
                    for (t = G->ladj[k]; t != G->z; t = t->next) {
                        if (d[t->v] < d[k] + t->wt) {
                            st[t->v] = k;
                            d[t->v] = d[k] + t->wt;
                        }
                    }
                    
                }
            }

            // Show max path from source i
            for (j = 0; j < G->V; j++) {
                k = j;
                if (j != i) {
                    printf("Max path from source %s to %s:\n", STsearchByIndex(G->tab, i), STsearchByIndex(G->tab, j));
                    printf("%s", STsearchByIndex(G->tab, k));
                    while (k != i) {
                        printf(" <- %s", STsearchByIndex(G->tab, st[k]));
                        k = st[k];
                    }
                    printf("\nTotal weight: %d\n", d[j]);
                }
            }
        }
    }

    free(d);
    free(st);
}
