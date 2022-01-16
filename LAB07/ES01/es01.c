/* 
    LAB07 ES01 RAINERI ANDREA ANGELO S280848
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

typedef struct diag {
    int elementi[MAX_ELEM]; // reference to element inside the elements array
    int N;                  // actual number of elements in diagonal
} Diagonale;

typedef struct progr {
    Diagonale diagonali[MAX_DIAG];
} Programma;

int readElements(char *filename, Elemento **el);
Boolean checkValidProgramma(Programma *progr, Elemento *elementi, int N);
void findBestProgramma(Programma *progr, Elemento *elementi, int N, int DD, int DP);
void displayProgramma(Programma *progr, Elemento *elementi, int N);

int main(int argc, char const *argv[]) {
    int N, DD = 10, DP = 25;
    Elemento *elementi;
    Programma progr;

    N = readElements("elementi.txt", &elementi);

    printf("--- Test Case #1 ---\n");
    printf("DD = %d DP = %d\n", DD, DP);
    findBestProgramma(&progr, elementi, N, DD, DP);
    displayProgramma(&progr, elementi, N);

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

Boolean checkValidProgramma(Programma *progr, Elemento *elementi, int N) {
    int i, j;
    Boolean acrobAvanti = false, acrobIndietro = false, foundAcrobInDiag;
    Boolean twoAcrobSeq = false;

    for (i = 0; i < MAX_DIAG; i++) {
        foundAcrobInDiag = false;

        // Controllo presenza diagonale (che sia riempita con almeno un elemento)
        if (progr->diagonali[i].N == 0)
            return false;
        
        for (j = 0; j < progr->diagonali[i].N; j++) {
            // Controllo tipo acrobatico
            if (elementi[progr->diagonali[i].elementi[j]].tipo > 0) {
                foundAcrobInDiag = true;
            }

            // Controllo tipo acrobatico avanti
            if (elementi[progr->diagonali[i].elementi[j]].tipo == ACROB_AVANTI) {
                acrobAvanti = true;
            }
            // Controllo tipo acrobatico indietro
            if (elementi[progr->diagonali[i].elementi[j]].tipo == ACROB_INDIETRO) {
                acrobIndietro = true;
            }
            
            // Controllo elementi acrobatici in sequenza
            if (j > 0) {
                if (elementi[progr->diagonali[i].elementi[j-1]].tipo > 0 && elementi[progr->diagonali[i].elementi[j]].tipo > 0) {
                    twoAcrobSeq = true;
                }
            }

            if (j != progr->diagonali[i].N-1) {
                if (elementi[progr->diagonali[i].elementi[j]].finale == true)
                    return false;
            }
        }

        if (!foundAcrobInDiag)
            return false;

    }

    if (!acrobAvanti || !acrobIndietro)
        return false;

    if (!twoAcrobSeq)
        return false;

}

Boolean calculatePointsDiag(Elemento *elementi, int N, Diagonale diag, float *pt, int numDiag) {
    int i;
    Boolean bonus = false;
    float tmp = 0;

    for (i = 0; i < diag.N; i++) {
        tmp += elementi[diag.elementi[i]].valore;
        if (elementi[diag.elementi[i]].finale && elementi[diag.elementi[i]].diff >= 8 && numDiag == MAX_DIAG - 1) {
            bonus = true;
        }
    }
    *pt = tmp;
    return bonus;
}

float calculatePoints(Programma *progr, Elemento *elementi, int N) {
    int i;
    Boolean bonus = false;
    float pt = 0, tmp;

    for (i = 0; i < MAX_DIAG; i++) {
        bonus = calculatePointsDiag(elementi, N, progr->diagonali[i], &tmp, i);
        if (bonus)
            tmp = tmp * 1.5;
        pt += tmp;
    }
    return pt;
}

void bestProgrammaR(int pos1, int pos2, Elemento *elementi, int N, int DD, int DP, Programma *progr, Programma *bProgr, float *ptBProgr, int diffTot, int diffDiag) {
    int i;
    float ptProgr;

    // Condizione di terminazione -- check programma
    if (pos1 == MAX_DIAG) {
        if (checkValidProgramma(progr, elementi, N)) {
            ptProgr = calculatePoints(progr, elementi, N);
            if (ptProgr > *ptBProgr) {
                *bProgr = *progr;
                *ptBProgr = ptProgr;
            }
        }
        return;
    }

    if (pos2 == MAX_ELEM)
        bestProgrammaR(pos1+1, 0, elementi, N, DD, DP, progr, bProgr, ptBProgr, diffTot, 0);


    for (i = 0; i < N; i++) {
        if (((diffTot + elementi[i].diff) <= DP) && ((diffDiag + elementi[i].diff) <= DD)) {
            if (pos2 == 0) {
                if (elementi[i].dirIngresso == FRONTE && elementi[i].reqPreced == false) {
                    progr->diagonali[pos1].elementi[pos2] = i;
                    progr->diagonali[pos1].N++;
                    diffTot += elementi[i].diff;
                    diffDiag += elementi[i].diff;
                    bestProgrammaR(pos1, pos2+1, elementi, N, DD, DP, progr, bProgr, ptBProgr, diffTot, diffDiag);
                    progr->diagonali[pos1].N--;
                    diffTot -= elementi[i].diff;
                    diffDiag -= elementi[i].diff;
                }
            } else {
                if (elementi[i].dirIngresso == elementi[progr->diagonali[pos1].elementi[pos2-1]].dirUscita) {
                    progr->diagonali[pos1].elementi[pos2] = i;
                    progr->diagonali[pos1].N++;
                    diffTot += elementi[i].diff;
                    diffDiag += elementi[i].diff;
                    bestProgrammaR(pos1, pos2+1, elementi, N, DD, DP, progr, bProgr, ptBProgr, diffTot, diffDiag);
                    progr->diagonali[pos1].N--;
                    diffTot -= elementi[i].diff;
                    diffDiag -= elementi[i].diff;
                }
            }
        }
        
    }
    bestProgrammaR(pos1+1, 0, elementi, N, DD, DP, progr, bProgr, ptBProgr, diffTot, 0);

}

Programma emptyProgramma() {
    Programma progr;
    int i;
    for (i = 0; i < MAX_DIAG; i++) {
        progr.diagonali[i].N = 0;
    }
    return progr;
}

void findBestProgramma(Programma *progr, Elemento *elementi, int N, int DD, int DP) {
    float bestPt = 0;
    Programma bestProgramma, progrTmp;
    bestProgramma = progrTmp = emptyProgramma();

    bestProgrammaR(0, 0, elementi, N, DD, DP, &progrTmp, &bestProgramma, &bestPt, 0, 0);

    *progr = bestProgramma;
}

void displayProgramma(Programma *progr, Elemento *elementi, int N) {
    int i, j;
    Boolean bonus;
    float pt, bestPt;

    bestPt = calculatePoints(progr, elementi, N);

    printf("TOT: %.3f\n", bestPt);

    for (i = 0; i < MAX_DIAG; i++) {
        bonus = calculatePointsDiag(elementi, N, progr->diagonali[i], &pt, i);
        printf("DIAG#%d > %.3f", i+1, pt);
        if (bonus) {
            printf(" * 1.5 (BONUS)");
        }
        printf("\n");
        for (j = 0; j < progr->diagonali[i].N; j++) {
            printf("%s ", elementi[progr->diagonali[i].elementi[j]].nome);
        }
        printf("\n");
    }
}
