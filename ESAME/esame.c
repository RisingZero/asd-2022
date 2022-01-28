#include <stdio.h>
#include <stdlib.h>

void readFile(FILE *fin, int ***griglia, int *R, int *C);
void verificaProposta(FILE *fin, int **griglia, int R, int C);
void resetGriglia(int **griglia, int R, int C);
int check(int **griglia, int R, int C);
void free2d(int ***griglia, int R, int C);
void findBestCopertura(int **griglia, int R, int C);
int findCopertura(int pos, int *sol, int *dx, int **griglia, int R, int C, int n);

int main(void) {
    FILE *fp;
    int **griglia, R, C;

    if ((fp = fopen("griglia.txt", "r")) == NULL) {
        printf("Errore apertura file input");
        return 1;
    }
    readFile(fp, &griglia, &R, &C);
    fclose(fp);

    if ((fp = fopen("proposta.txt", "r")) == NULL) {
        printf("Errore apertura file proposta");
        return 2;
    }
    verificaProposta(fp, griglia, R, C);
    fclose(fp);

    findBestCopertura(griglia, R, C);
    free2d(&griglia, R, C);

    return 0;
}

void readFile(FILE *fin, int ***griglia, int *R, int *C) {
    int i, j;

    fscanf(fin, "%d %d", R, C);
    (*griglia) = (int **) malloc(*R * sizeof(int*));
    for (i = 0; i < *R; i++) {
        (*griglia)[i] = (int *) malloc(*C * sizeof(int));
        for (j = 0; j < *C; j++) {
            fscanf(fin, "%d", &(*griglia)[i][j]);
        }
    }
}

void verificaProposta(FILE *fin, int **griglia, int R, int C) {
    int n, i, j, pos, k, dx, dy, valid = 1;
    n = 0;
    
    while(fscanf(fin, "%d %d %d", &pos, &dx, &dy) == 3) {
        if (dx != dy) {
            valid = 0;
            break;
        }
        for (i = 0; i < dy; i++) {
            for (j = 0; j < dx; j++) {
                griglia[pos/C + i][pos%C + j] = 2;
            }
        }
        n++;
    }
    if (valid) {
        valid = check(griglia, R, C);
    }

    if (valid) {
        printf("La copertura e' valida\n");
        printf("Usate %d regioni\n", n);
        resetGriglia(griglia, R, C);
        return;
    }

    printf("La copertura non e' valida\n");
    resetGriglia(griglia, R, C);
    return;
}

void resetGriglia(int **griglia, int R, int C) {
    int i, j;
    for (i = 0; i < R; i++) {
        for (j = 0; j < C; j++) {
            if (griglia[i][j] == 2)
                griglia[i][j] = 0;
        }
    }
}

int check(int **griglia, int R, int C) {
    int i, j;
    for (i = 0; i < R; i++) {
        for (j = 0; j < C; j++) {
            if (griglia[i][j] == 0)
                return 0;
        }
    }
    return 1;
}

void free2d(int ***griglia, int R, int C) {
    int i;
    for (i = 0; i < R; i++) {
        free((*griglia)[i]);
    }
    free(*griglia);
}

void findBestCopertura(int **griglia, int R, int C) {
    int *sol, *dx, *dy;
    int n, i;

    sol = (int *) malloc(R*C * sizeof(int));
    dx = (int *) malloc(R*C * sizeof(int));

    for (i = 0; i < R*C; i++) {
        sol[i] = -1; dx[i] = 0;
    }

    for (n = 1; n <= R*C; n++) {
        if (findCopertura(0, sol, dx, griglia, R, C, n))
            break;
    }

    printf("La copertura ottima e' la seguente: \n");

    // STAMPA COPERTURA OTTIMA
    for (i = 0; i < R*C; i++) {
        if (sol[i] != -1) {
            printf("Coordinate regione: %d %d, dimensione %d\n", sol[i]/C, sol[i]%C, dx[i]);
        }
    }
    free(sol);
    free(dx);
}

int findCopertura(int pos, int *sol, int *dx, int **griglia, int R, int C, int n) {
    int i, j, d, validDim;

    if (pos >= R*C || n == 0) {
        return check(griglia, R, C);
    }

    // Skip cella già occupata
    if (griglia[pos/C][pos%C] > 0)
        return findCopertura(pos+1, sol, dx, griglia, R, C, n);

    for (d = 1; d <= R && d <= C; d++) {
        validDim = 1;
        for (i = 0; i < d && validDim; i++) {
            for (j = 0; j < d && validDim; j++) {
                if ((pos/C + i) >= R || (pos%C + j) >= C || griglia[pos/C + i][pos%C + j] > 0)
                    validDim = 0;
            }
        }

        if (validDim) {
            sol[pos] = pos;             // Scelta posizione e dimensione regione
            dx[pos] = d;
            for (i = 0; i < d; i++) {
                for (j = 0; j < d; j++) {
                    griglia[pos/C + i][pos%C + j] = 2;
                }
            }

            if (findCopertura(pos+1, sol, dx, griglia, R, C, n-1))     // Inserimento di n-1 regioni nello spazio restante
                return 1;

            for (i = 0; i < d; i++) {
                for (j = 0; j < d; j++) {
                    griglia[pos/C + i][pos%C + j] = 0;
                }
            }
            sol[pos] = -1;
            dx[pos] = 0;
        }
    }
    return 0;
}