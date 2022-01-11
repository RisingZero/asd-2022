#ifndef GRAPH_MODULE
#define GRAPH_MODULE

/* Weighted - oriented graph module */

#include <stdio.h>
#include <stdlib.h>

typedef struct edge { int v; int w; int wt; } Edge;
typedef struct graph *Graph;

Graph GRAPHinit(int V);
void GRAPHfree(Graph G);

Graph GRAPHload(FILE *fin);
void GRAPHstore(Graph G, FILE *fout);

int GRAPHgetIndex(Graph G, char *label);

void GRAPHinsertE(Graph G, int id1, int id2, int wt);
void GRAPHremoveE(Graph G, int id1, int id2, int wt);
void GRAPHedges(Graph G, Edge *a);

void GRAPHfindESubsetToDAG(Graph G);

#endif