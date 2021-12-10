/* 
    LAB06 ES01 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI  A.A. 2021/22
 */

/* 
    DIMOSTRAZIONE DI APPLICABILITÀ P.D.
    -> il problema da risolvere è un problema di ottimizzazione
    -> il problema gode di sottostruttura ottima, infatti:
    
    Supponiamo al passo i  di avere ottenuto una soluzione ottima,
    il tempo di attività è dato da t_i + (f-s)_i, dove t_i è il tempo
    di attività al passo precedente la scelta i-esima.
    Se la scelta precedente non fosse una scelta ottima allora esisterebbe
    t_i' > t_i, dunque si avrebbe t_i'+ (f-s)_i > t_i + (f-s)_i,
    in conflitto con l'ipotesi iniziale che la soluzione i-esima fosse ottima.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int s, f;
} att;

void readFile(att **val, int *N, char *filename);
void ordAtt(int N, att *v);
int compatibile(att x, att y);
void bestActivitySequence(int N, att *v);
void stampaSoluzione(att *v, int *prev, int lastAtt);

int main(int argc, char const *argv[])
{
    att *val;
    int N;

    readFile(&val, &N, "att.txt");
    bestActivitySequence(N, val);

    readFile(&val, &N, "att1.txt");
    bestActivitySequence(N, val);

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

    if ((*val = (att *) malloc(*N * sizeof(att))) == NULL) {
        printf("ERRORE memoria esaurita");
        exit(2);
    }
    for (i = 0; i < *N; i++) {
        fscanf(fp, "%d %d", &((*val)[i].s), &((*val)[i].f));
    }

    fclose(fp);
}

void ordAtt(int N, att *v) {
    
}

int compatibile(att x, att y) {
    return !(x.s < y.f &&  y.s < x.f);
}

void bestActivitySequence(int N, att *v) {
    int *time, *prev;
    int i, j, best = 0, lastAtt = -1;

    if ((time = (int*) malloc(sizeof(int))) == NULL) {
        printf("ERRORE memoria esaurita");
        exit(2);
    }
    if ((prev = (int*) malloc(sizeof(int))) == NULL) {
        printf("ERRORE memoria esaurita");
        exit(2);
    }

    ordAtt(N, v);
    prev[0] = -1; time[0] = (v[0].f - v[0].s);
    for (i = 1; i < N; i++) {
        prev[i] = -1;
        time[i] = (v[i].f - v[i].s);

        for (j = 0; j < i; j++) {
            if (compatibile(v[i], v[j]) && (time[i] < time[j] + (v[i].f - v[i].s))) {
                prev[i] = j; time[i] = time[j] + (v[i].f - v[i].s);
            }
        }
        if (best < time[i]) {
            best = time[i]; 
            lastAtt = i;
        }
    }

    stampaSoluzione(v, prev, lastAtt);

    free(prev); free(time);
}

void stampaSoluzione(att *v, int *prev, int lastAtt) {
    if (lastAtt == -1) {
        printf("\n");
        return;
    }

    stampaSoluzione(v, prev, prev[lastAtt]);
    printf("(%d, %d) ", v[lastAtt].s, v[lastAtt].f);
}