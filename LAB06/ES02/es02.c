/* 
    LAB04 ES03 S280848 RAINERI ANDREA ANGELO
    ALGORITMI E STRUTTURE DATI  
 */

/* 
    LONGEST NECKLACE 
    P.D. MEMOIZATION VERSION
    Data structure containing already evalueted solutions:
    int memo[Z][R][T][S][4];
    each cell contains the max length of a necklace built with z,r,t,s stones, starting with one of the 4 stones (last dim)
 */

#include <stdio.h>
#include <stdlib.h>

#define STONES_TYPES 4
#define FILENAME1 "easy_test_set.txt"
#define FILENAME2 "hard_test_set.txt"

typedef enum { Z, R, T, S } stoneTypes;
void printNecklace(int *sol, int k, char *stones);
int fZ(int numStones[STONES_TYPES], int *****memo);
int fR(int numStones[STONES_TYPES], int *****memo);
int fT(int numStones[STONES_TYPES], int *****memo);
int fS(int numStones[STONES_TYPES], int *****memo);
void matAlloc(int ******memo, int numStones[STONES_TYPES]);
void matFree(int *****memo, int numStones[STONES_TYPES]);

int main(int argc, char const *argv[])
{
    FILE *fp;
    char stones[STONES_TYPES] = {'Z', 'R', 'T', 'S'};
    int i, j, n, numStones[STONES_TYPES] = {0};
    int *****memo;
    int t, best;

    // File reading
    if ((fp = fopen(FILENAME2, "r")) == NULL) {
        printf("ERRORE apertura file di input");
        exit(1);
    }
    fscanf(fp, "%d", &n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < STONES_TYPES; j++) {
            fscanf(fp, "%d", &numStones[j]);
        }
        printf("%d) ", i);

        // Multidim matrix allocation
        matAlloc(&memo, numStones);
        best = t = fZ(numStones, memo);
        if ((t = fR(numStones, memo)) > best)
            best = t;
        if ((t = fT(numStones, memo)) > best)
            best = t;
        if ((t = fS(numStones, memo)) > best)
            best = t;

        printf("Migliore soluzione: %d pietre\n", best);  

        matFree(memo, numStones);

    }
    fclose(fp);


    return 0;
}
 
void matAlloc(int ******memo, int numStones[STONES_TYPES]) {
    int i, j, k, t;

    if ((*memo = (int *****) malloc((numStones[Z]+1) * sizeof(int ****))) == NULL) {
        printf("ERRORE memoria esaurita");
        exit(2);
    }
    for (i = 0; i <= numStones[Z]; i++) {
        if (((*memo)[i] = (int ****) malloc((numStones[R]+1) * sizeof(int ***))) == NULL) {
            printf("ERRORE memoria esaurita");
            exit(2);
        }
        for (j = 0; j <= numStones[R]; j++) {
            if (((*memo)[i][j] = (int ***) malloc((numStones[T]+1) *  sizeof(int **))) == NULL) {
                printf("ERRORE memoria esaurita");
                exit(2);
            }
            for (k = 0; k <= numStones[T]; k++) {
                if (((*memo)[i][j][k] = (int **) malloc((numStones[S]+1) * sizeof(int *))) == NULL) {
                    printf("ERRORE memoria esaurita");
                    exit(2);
                }
                for (t = 0; t <= numStones[S]; t++) {
                    if (((*memo)[i][j][k][t] = (int *) calloc(STONES_TYPES, sizeof(int))) == NULL) {
                        printf("ERRORE memoria esaurita");
                        exit(2);
                    }
                }
            }
        }
    }
}

void matFree(int *****memo, int numStones[STONES_TYPES]) {
    int i, j, k, t;

    for (i = 0; i <= numStones[Z]; i++) {
        for (j = 0; j <= numStones[R]; j++) {
            for (k = 0; k <= numStones[T]; k++) {
                for (t = 0; t <= numStones[S]; t++) {
                    free(memo[i][j][k][t]);
                }
                free(memo[i][j][k]);
            }
            free(memo[i][j]);
        }
        free(memo[i]);
    }
    free(memo);
}

/* 
    Validity conditions:
    Z -> Z / R
    T -> Z / R
    R -> S / T
    S -> S / T
    Matrix dim order: Z R T S <first stone>
 */
int fZ(int numStones[STONES_TYPES], int *****memo) {
    int r1, r2, best;
    if (memo[numStones[Z]][numStones[R]][numStones[T]][numStones[S]][Z] > 0)
        return memo[numStones[Z]][numStones[R]][numStones[T]][numStones[S]][Z];
    else {
        if (numStones[Z] > 0) {
            numStones[Z]--;
            r1 = fZ(numStones, memo);
            r2 = fR(numStones, memo);
            numStones[Z]++;
            best = (r1 > r2) ? r1+1 : r2+1;
            memo[numStones[Z]][numStones[R]][numStones[T]][numStones[S]][Z] = best;

            return best;
        } else
            return 0;
    }
}

int fR(int numStones[STONES_TYPES], int *****memo) {
    int r1, r2, best;
    if (memo[numStones[Z]][numStones[R]][numStones[T]][numStones[S]][R] > 0)
        return memo[numStones[Z]][numStones[R]][numStones[T]][numStones[S]][R];
    else {
        if (numStones[R] > 0) {
            numStones[R]--;
            r1 = fS(numStones, memo);
            r2 = fT(numStones, memo);
            numStones[R]++;
            best = (r1 > r2) ? r1+1 : r2+1;
            memo[numStones[Z]][numStones[R]][numStones[T]][numStones[S]][R] = best;

            return best;
        } else
            return 0;
    }
}

int fT(int numStones[STONES_TYPES], int *****memo) {
    int r1, r2, best;
    if (memo[numStones[Z]][numStones[R]][numStones[T]][numStones[S]][T] > 0)
        return memo[numStones[Z]][numStones[R]][numStones[T]][numStones[S]][T];
    else {
        if (numStones[T] > 0) {
            numStones[T]--;
            r1 = fZ(numStones, memo);
            r2 = fR(numStones, memo);
            numStones[T]++;
            best = (r1 > r2) ? r1+1 : r2+1;
            memo[numStones[Z]][numStones[R]][numStones[T]][numStones[S]][T] = best;

            return best;
        } else
            return 0;
    }
}

int fS(int numStones[STONES_TYPES], int *****memo) {
    int r1, r2, best;
    if (memo[numStones[Z]][numStones[R]][numStones[T]][numStones[S]][S] > 0)
        return memo[numStones[Z]][numStones[R]][numStones[T]][numStones[S]][S];
    else {
        if (numStones[S] > 0) {
            numStones[S]--;
            r1 = fS(numStones, memo);
            r2 = fT(numStones, memo);
            numStones[S]++;
            best = (r1 > r2) ? r1+1 : r2+1;
            memo[numStones[Z]][numStones[R]][numStones[T]][numStones[S]][S] = best;

            return best;
        } else
            return 0;
    }
}