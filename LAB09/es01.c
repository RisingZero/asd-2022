/* 
    LAB09 ES01 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI A.A.2021
 */

#include <stdio.h>

#include "GRAPH.h"

int main(int argc, char const *argv[])
{
    Graph G, dag;
    FILE *fp;
    Edge *bestEdgesSet;
    int bestCardinality, i;

    if ((fp = fopen("grafo2.txt", "r")) == NULL) {
        printf("Error opening file");
        exit(1);
    }

    G = GRAPHload(fp);
    fclose(fp);

    bestEdgesSet = (Edge *)malloc(GRAPHedgeCount(G) * sizeof(Edge));
    GRAPHfindESubsetToDAG(G, bestEdgesSet, &bestCardinality);

    dag = GRAPHcreateFromGraphEdgeSubtraction(G, bestEdgesSet);

    GRAPHstore(dag, stdout);

    GRAPHfree(G);
    GRAPHfree(dag);
    return 0;
}
