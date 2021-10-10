/* 
    LAB02 ES02 S280848 RAINERI ANDREA ANGELO
    ALGORITMI E STRUTTURE DATI  
 */

#include <stdio.h>
#include <stdlib.h>

#define FILENAME "mat.txt"

void readFile(int ***mat, int *nr, int *nc);
void malloc2dP(int ***mat, int nr, int nc);
int **malloc2dR(int nr, int nc);
void free2d(int **mat, int nr);
void separa(int **mat, int nr, int nc, int *white, int *wDim, int *black, int *bDim);

int main(void) {

    int **mat, nr, nc;
    int *white, *black, wDim, bDim;
    int i;

    readFile(&mat, &nr, &nc);

    if ((white = (int *) malloc(nr * nc / 2 * sizeof(int))) == NULL) {
        printf("ERROR allocating 'white' array");
        exit(2);
   }

    if ((black = (int *) malloc((nr*nc - nr*nc/2) * sizeof(int))) == NULL) {
        printf("ERROR allocating 'black' array");
        exit(2);
   }

    separa(mat, nr, nc, white, &wDim, black, &bDim);

    printf("Black:\n");
    for (i = 0; i < bDim; i++) {
        printf("%d ", black[i]);
    }
    printf("\nWhite:\n");
    for (i = 0; i < wDim; i++) {
        printf("%d ", white[i]);
    }

    free(white);
    free(black);
    free2d(mat, nr);

    return 0;
}

void readFile(int ***mat, int *nr, int *nc) {
    FILE *fp;
    int i, j;

    if ((fp = fopen(FILENAME, "r")) == NULL) {
        printf("ERRORE lettura file!");
        exit(1);
    }

    fscanf(fp, "%d %d", nr, nc);
    
    malloc2dP(mat, *nr, *nc);
    
    for (i = 0; i < *nr; i++) {
        for (j = 0; j < *nc; j++) {
            fscanf(fp, "%d", &(*mat)[i][j]);
        }
    }

    fclose(fp);
}

void malloc2dP(int ***mat, int nr, int nc) {
    int i;

    *mat = (int**) malloc(nr * sizeof(int*));
    if (*mat == NULL) {
        printf("ERRORE: memoria esaurita!");
        exit(2);
    }
    for (i = 0; i < nr; i++) {
        (*mat)[i] = (int *) malloc(nc * sizeof(int));
        if ((*mat)[i] == NULL) {
            printf("ERRORE: memoria esaurita!");
            exit(2);
        }
    }
}

int **malloc2dR(int nr, int nc) {
    int **mat, i;

    mat = (int**) malloc(nr * sizeof(int*));
    if (mat == NULL) {
        printf("ERRORE: memoria esaurita!");
        exit(2);
    }
    for (i = 0; i < nr; i++) {
        mat[i] = (int *) malloc(nc * sizeof(int));
        if (mat[i] == NULL) {
            printf("ERRORE: memoria esaurita!");
            exit(2);
        }
    }

    return mat;
}

void free2d(int **mat, int nr) {
    int i;

    for (i = 0; i < nr; i++) {
        free(mat[i]);
    }
    free(mat);
}

void separa(int **mat, int nr, int nc, int *white, int *wDim, int *black, int *bDim) {
    int i, j, *w, *b;

    w = white; b = black;
    for (i = 0 ; i < nr;  i++) {
        for (j = 0; j < nc; j++) {
            if ((i+j)%2 == 0) {
                *(b++) = mat[i][j];
            } else {
                *(w++) = mat[i][j];
            }
        }
    }

    *wDim = w-white; 
    *bDim = b-black;
}