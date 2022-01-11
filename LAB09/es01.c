/* 
    LAB09 ES01 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI A.A.2021
 */

#include <stdio.h>

#include "GRAPH.h";

int main(int argc, char const *argv[])
{
    Graph G;
    FILE *fp;

    if ((fp = fopen("grafo2.txt", "r")) == NULL) {
        printf("Error opening file");
        exit(1);
    }

    G = GRAPHload(fp);
    fclose(fp);

    if ((fp = fopen("out.txt", "w")) == NULL) {
        printf("Error opening output file");
        exit(2);
    }
    GRAPHstore(G, fp);
    fclose(fp);

    return 0;
}
