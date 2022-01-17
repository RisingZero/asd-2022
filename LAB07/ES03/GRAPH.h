#ifndef GRAPH_MODULE
#define GRAPH_MODULE

#include "ST.h"
#include "Item.h"

typedef struct graph *Graph;

Graph GRAPHinit(int V);
void GRAPHfree(Graph G);

void GRAPHinsertE(Graph G, char *name1, char *subnet1, char *name2, char *subnet2, int wt);

void GRAPHbuildLadj(Graph G);

void GRAPHdisplayVertexEdgesOrdered(Graph G);

void GRAPHthreeVAdjacency(Graph G);

#endif