/* 
    LAB05 ES01 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI 2021/22
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int s, f;
} att;

void readFile(att **val, int *N, char *filename);
void attSel(int N, att *v);
void attSelR(int pos, int *sol, int *B_sol, att *val, int N, int *d, int *B_d);
int isCompatibile(att *val, int N, int pos, int *sol);
void stampaSoluzione(int *sol, att *val, int N);  

int main(int argc, char const *argv[])
{
    att *val;
    int N;

    readFile(&val, &N, "att.txt");
    attSel(N, val);

    readFile(&val, &N, "att1.txt");
    attSel(N, val);

    free(val);

    return 0;
}

void readFile(att **val, int *N, char *filename) {
    FILE *fp;
    int i;

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("ERRORE apertura file di input");
        exit(1);
    }

    fscanf(fp, "%d", N);

    *val = (att *) malloc(*N * sizeof(att));
    for (i = 0; i < *N; i++) {
        fscanf(fp, "%d %d", &((*val)[i].s), &((*val)[i].f));
    }

    fclose(fp);
}

void attSel(int N, att *v) {
    int *sol, *B_sol;
    int d = 0, B_d = 0;

    sol = (int *) calloc(N, sizeof(int));
    B_sol = (int *) calloc(N, sizeof(int));

    attSelR(0, sol, B_sol, v, N, &d, &B_d);
    printf("Migliore soluzione:\n");
    stampaSoluzione(B_sol, v, N);
    printf("Durata migliore: %d\n", B_d);

    free(sol);
    free(B_sol);
}

void attSelR(int pos, int *sol, int *B_sol, att *val, int N, int *d, int *B_d) {
    int i;
    // Terminazione
    if (pos >= N) {
        if (*d > *B_d) {
            *B_d = *d;
            // copia soluzione nuova in B_sol
            for (i = 0; i < N; i++) {
                B_sol[i] = sol[i];
            }
        }
        return;
    }
    if (isCompatibile(val, N, pos, sol)) {
        sol[pos] = 1;
        *d += (val[pos].f - val[pos].s);
        attSelR(pos+1, sol, B_sol, val, N, d, B_d);
        *d -= (val[pos].f - val[pos].s);
    }
    
    sol[pos] = 0;
    attSelR(pos+1, sol, B_sol, val, N, d, B_d);

    return;
}

int isCompatibile(att *val, int N, int pos, int *sol) {
    int ok = 1, i;
    att el = val[pos];
    att x;

    for (i = 0; i < N && ok; i++) {
        if (i != pos && sol[i] == 1) {
            if (val[i].s < el.f && el.s < val[i].f)
                ok = 0;
        }
    }
    return ok;
}

void stampaSoluzione(int *sol, att *val, int N) {   
    int i;

    for (i = 0; i < N; i++) {
        if (sol[i] == 1) {
            printf("(%d %d) ", val[i].s, val[i].f);
        }
    }
    printf("\n");
}
