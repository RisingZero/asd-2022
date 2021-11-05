/* 
    LAB04 ES01 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI
 */

#include <stdio.h>
#include <stdlib.h>
#define FILENAME "graph.txt"

void readGraph(int ***arches, int *N, int *E);
void vertexCovers(int **arches, int N, int E);
void vertexCoverR(int pos, int **arches, int E, int *sol, int N);
int checkVertexCover(int *sol, int N, int **arches, int E);

int main(int argc, char const *argv[])
{
    int **arches, N, E;

    readGraph(&arches, &N, &E);
    vertexCovers(arches, N, E);

    return 0;
}

void readGraph(int ***arches, int *N, int *E) {
    FILE *fp;

    int i;
    int u, v;

    // File open
    if ((fp = fopen(FILENAME, "r")) == NULL) {
        printf("Errore apertura file di input");
        exit(1);
    }

    // Incidence matrix allocation
    fscanf(fp, "%d %d", N, E);
    *arches = (int **) malloc(*E * sizeof(int *));
    if (*arches == NULL) {
        printf("Errore, memoria esaurita");
        exit(2);
    }
    for (i = 0; i < *E; i++) {
        (*arches)[i] = (int *) malloc(2 * sizeof(int));
        if ((*arches)[i] == NULL) {
            printf("Errore, memoria esaurita");
            exit(2);
        }
        fscanf(fp, "%d %d", &(*arches)[i][0], &(*arches)[i][1]);
    }
    fclose(fp);
}

/* 
    Wrapper function for the recursive implementation
 */
void vertexCovers(int **arches, int N, int E) {
    int k, *sol;
    sol = (int *) malloc(N * sizeof(int));
    vertexCoverR(0, arches, E, sol, N);
    free(sol);
}

void vertexCoverR(int pos, int **arches, int E, int *sol, int N) {
    int i;

    if (pos >= N) {
        if (checkVertexCover(sol, N, arches, E)) {
            printf("( ");
            for (i = 0; i < N; i++) {
                if (sol[i] == 1) {
                    printf("%d ", i);
                }
            }
            printf(")\n");
        }
        return;
    }

    sol[pos] = 0;
    vertexCoverR(pos+1, arches, E, sol, N);
    sol[pos] = 1;
    vertexCoverR(pos+1, arches, E, sol, N);
}

int checkVertexCover(int *sol, int N, int **arches, int E) {
    int i, j;

    for (i = 0; i < E; i++) {
        if (sol[arches[i][0]] == 0 && sol[arches[i][1]] == 0)
            return 0;
    }
    return 1;
}