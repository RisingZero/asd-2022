#ifndef GRAPH_ADT_MODULE
#define GRAPH_ADT_MODULE

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

void GRAPHpath(Graph G, int id1, int id2);
void GRAPHpathH(Graph G, int id1, int id2);
void GRAPHbfs(Graph G, int id);
void GRAPHdfs(Graph G, int id);
int GRAPHcc(Graph G);
int GRAPHscc(Graph G);

#endif