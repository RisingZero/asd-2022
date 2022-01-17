/* 
    LAB07 ES02 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI  A.A.2021
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME_LENGTH 100
#define MAX_ELEM 5
#define MAX_DIAG 3

typedef enum bool { false, true } Boolean;

enum tipo { TRANSIZIONE, ACROB_INDIETRO, ACROB_AVANTI };
enum direzione { SPALLE, FRONTE };

typedef struct elem {
    char nome[MAX_NAME_LENGTH+1];
    int tipo;
    int dirIngresso;
    int dirUscita;
    Boolean reqPreced;
    Boolean finale;
    float valore;
    int diff;
} Elemento;

typedef struct params {
    Boolean needAcrobInDiag;
    Boolean needAcrobAv;
    Boolean needAcrobInd;
    Boolean acrobInSeq;
    Boolean needAcrobInSeq;
} ObjFuncParams;

int readElements(char *filename, Elemento **el);
void findBestProgrammaGreedy(Elemento *elementi, int N, int DD, int DP);
int getAccettabili(Elemento *accettabili, Elemento *elementi, int N, int DD, int DP, Elemento *sol, int pos, int posInDiag, int currKp, int *diffDiags, int diffTot, ObjFuncParams params);
void sortAccettabili(Elemento *accettabili, int N, int *sol, int pos, int currKp, ObjFuncParams *params);
float objFunc(Elemento el, ObjFuncParams params);
int compareFunc(void *_context, const void *a, const void *b);
void stampaSoluzione(Elemento *sol, int N, int *kp);

int main(int argc, char const *argv[]) {
    int N, DD = 12, DP = 30;
    Elemento *elementi;

    N = readElements("elementi.txt", &elementi);

    printf("--- Test Case #1 ---\n");
    printf("DD = %d DP = %d\n", DD, DP);
    findBestProgrammaGreedy(elementi, N, DD, DP);

    free(elementi);
    return 0;
}

int readElements(char *filename, Elemento **el) {
    int N, i;
    FILE *fp;

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("Errore apertura file di input\n");
        exit(1);
    };

    fscanf(fp, "%d", &N);
    (*el) = (Elemento *) malloc(N * sizeof(Elemento));
    for (i = 0; i < N; i++) {
        fscanf(fp, "%s %d %d %d %d %d %f %d",
            (*el)[i].nome,
            &(*el)[i].tipo,
            &(*el)[i].dirIngresso,
            &(*el)[i].dirUscita,
            &(*el)[i].reqPreced,
            &(*el)[i].finale,
            &(*el)[i].valore,
            &(*el)[i].diff
        );
    }

    fclose(fp);

    return N;
}

void findBestProgrammaGreedy(Elemento *elementi, int N, int DD, int DP) {
    int *kp, diffDiags[MAX_DIAG] = {0};
    int i, pos, posInDiag, currKp, diffTot, numAcc, minAv = DD, minInd = DD;
    Elemento *accettabili, *sol;
    ObjFuncParams params;
    Boolean found;

    for (i = 0; i < N; i++) {
        if (elementi[i].diff < minAv && elementi[i].tipo == ACROB_AVANTI)
            minAv = elementi[i].diff;
        if (elementi[i].diff < minInd && elementi[i].tipo == ACROB_INDIETRO)
            minInd = elementi[i].diff;
    }
    kp = (int *) malloc(MAX_DIAG * MAX_ELEM * sizeof(int));
    for (i = 0; i < MAX_DIAG * MAX_ELEM; i++)
        kp[i] = -1;
    sol = (Elemento *) malloc(MAX_DIAG * MAX_ELEM * sizeof(Elemento));
    accettabili = (Elemento *) malloc(N * sizeof(Elemento));
    params.needAcrobInSeq = params.acrobInSeq = false;
    params.needAcrobAv = params.needAcrobInd = params.needAcrobInDiag = true;

    diffTot = 0; posInDiag = 0; currKp = 0;
    for (pos = 0; pos < MAX_DIAG * MAX_ELEM; ) {
        currKp = pos / MAX_ELEM;
        numAcc = getAccettabili(accettabili, elementi, N, DD, DP, sol, pos, posInDiag, currKp, diffDiags, diffTot, params);
        if (numAcc > 0) {
            // Ordinamento descrescente delle scelte possibili
            sortAccettabili(accettabili, numAcc, sol, pos, currKp, &params);

            if (params.needAcrobAv != params.needAcrobInd) {
                found = false;
                if (params.needAcrobAv) {
                    for (i = 0; i < numAcc && !found; i++) {
                        if (accettabili[i].tipo == ACROB_AVANTI) {
                            found = true;
                            // Effettuo scelta localmente ottima
                            sol[pos] = accettabili[0];
                            kp[pos] = currKp;
                        }     
                    }
                    
                    if (!found) {
                        found = false;
                        for (i = 0; i < numAcc && !found; i++) {
                            if (accettabili[i].dirIngresso != accettabili[i].dirUscita && diffDiags[currKp] + accettabili[i].diff + minAv <= DD && !accettabili[i].finale) {
                                sol[pos] = accettabili[i];
                                kp[pos] = currKp;
                                found = true;
                            }
                        }
                        if (!found) {
                            // Effettuo scelta localmente ottima
                            sol[pos] = accettabili[0];
                            kp[pos] = currKp;
                        }
                    }
                }

                if (params.needAcrobInd) {
                    for (i = 0; i < numAcc && !found; i++) {
                        if (accettabili[i].tipo == ACROB_INDIETRO) {
                            found = true;
                            // Effettuo scelta localmente ottima
                            sol[pos] = accettabili[0];
                            kp[pos] = currKp;
                        }     
                    }
                    
                    if (!found) {
                        found = false;
                        for (i = 0; i < numAcc && !found; i++) {
                            if (accettabili[i].dirIngresso != accettabili[i].dirUscita && diffDiags[currKp] + accettabili[i].diff + minInd <= DD && !accettabili[i].finale) {
                                sol[pos] = accettabili[i];
                                kp[pos] = currKp;
                                found = true;
                            }
                        }
                        if (!found) {
                            // Effettuo scelta localmente ottima
                            sol[pos] = accettabili[0];
                            kp[pos] = currKp;
                        }
                    }
                }
            } else {
                // Effettuo scelta localmente ottima
                sol[pos] = accettabili[0];
                kp[pos] = currKp;
            }

            // Aggiornamento difficoltà
            diffDiags[currKp] += sol[pos].diff;
            diffTot += sol[pos].diff;

            // Aggiornamento parametri funzione obiettivo
            if (sol[pos].tipo == ACROB_AVANTI) {
                params.needAcrobAv = false;
                params.needAcrobInDiag = false;
            }
            if (sol[pos].tipo == ACROB_INDIETRO) {
                params.needAcrobInd = false;
                params.needAcrobInDiag = false;
            }

            if (posInDiag > 0 && sol[pos].tipo > 0 && sol[pos-1].tipo > 0) {
                params.acrobInSeq = true;
            }

            if (posInDiag > 0 && sol[pos-1].tipo > 0 && !params.acrobInSeq) {
                params.needAcrobInSeq = true;
            } else
                params.needAcrobInSeq = false;

            if (sol[pos].finale) {
                // Skip a prossima diagonale se è stato inserito un elemento finale 
                pos = (currKp + 1) * MAX_ELEM;
                params.needAcrobInDiag = true;
                posInDiag = 0;
            } else {
                pos++;
                posInDiag++;
            }
        } else {
            // Skip a prossima diagonale in caso di impossibilità di scelta
            pos = (currKp + 1) * MAX_ELEM;
            params.needAcrobInDiag = true;
            posInDiag = 0;
        }
    }

    stampaSoluzione(sol, MAX_DIAG * MAX_ELEM, kp);

    free(accettabili);
    free(sol);
    free(kp);
    return;
}

int getAccettabili(Elemento *accettabili, Elemento *elementi, int N, int DD, int DP, Elemento *sol, int pos, int posInDiag, int currKp, int diffDiags[MAX_DIAG], int diffTot, ObjFuncParams params) {
    int i, cnt = 0;
    Boolean acceptable;
    Elemento el;

    for (i = 0; i < N; i++) {
        acceptable = true;
        el = elementi[i];

        if (diffDiags[currKp] + elementi[i].diff > DD || diffTot + elementi[i].diff > DP)
            acceptable = false;

        if (posInDiag == 0 && elementi[i].dirIngresso != FRONTE)
            acceptable = false;

        if (posInDiag > 0 && elementi[i].dirIngresso != sol[pos-1].dirUscita)
            acceptable = false;

        if (posInDiag == 0 && elementi[i].reqPreced)
            acceptable = false;

        if (acceptable)
            accettabili[cnt++] = el;
    }

    return cnt;
}

void sortAccettabili(Elemento *accettabili, int N, int *sol, int pos, int currKp, ObjFuncParams *params) {
    qsort_s(accettabili, N, sizeof(Elemento), compareFunc, params);
}

float objFunc(Elemento el, ObjFuncParams params) {
    float v;

    v = (el.valore / el.diff);

    if (el.tipo == ACROB_AVANTI && params.needAcrobAv) {
        v += 1;
    }

    if (el.tipo == ACROB_INDIETRO && params.needAcrobInd) {
        v += 1;
    }

    if (el.tipo > 0 && params.needAcrobInDiag) {
        v += 1;
    }

    if (el.tipo > 0 && params.needAcrobInSeq) {
        v += 1;
    }

    return v;
}

int compareFunc(void *_context, const void *a, const void *b) {
    ObjFuncParams params = *(ObjFuncParams *)_context;
    Elemento a1 = *(Elemento *)a;
    Elemento b1 = *(Elemento *)b;

    float va, vb;

    va = objFunc(a1, params) * 1000;
    vb = objFunc(b1, params) * 1000;

    return (vb - va);
}

void stampaSoluzione(Elemento *sol, int N, int *kp) {
    int i, j;
    int diffParz, diffTot = 0;
    float tot = 0, parz;

    for (j = 0; j < MAX_DIAG; j++) {
        parz = 0; diffParz = 0;
        for (i = 0; i < N; i++) {
            if (kp[i] == j) {
                diffParz += sol[i].diff;
                parz += sol[i].valore;
            }
        }
        tot += parz;
        diffTot += diffParz;
        printf("DIAG #%d > %.3f diff: %d\n", j+1, parz, diffParz);
        for (i = 0; i < N; i++) {
            if (kp[i] == j) {
                printf("%s ", sol[i].nome);
            }
        }
        printf("\n");
    }
    printf("TOT = %.3f DiffTot: %d", tot, diffTot);
}
