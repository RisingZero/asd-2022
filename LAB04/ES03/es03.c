/* 
    LAB04 ES03 S280848 RAINERI ANDREA ANGELO
    ALGORITMI E STRUTTURE DATI  
 */

#include <stdio.h>
#include <stdlib.h>

#define STONES_TYPES 4
#define FILENAME "easy_test_set.txt"

typedef enum { Z, R, T, S } stoneTypes;
void longestNecklace(char *stones, int *numStones);
int longestNecklaceR(int pos, int *sol, int k, int *numStones);
int check(int *sol, int k);
void printNecklace(int *sol, int k, char *stones);

int main(int argc, char const *argv[])
{
    FILE *fp;
    char stones[STONES_TYPES] = {'Z', 'R', 'T', 'S'};
    int i, j, n, numStones[STONES_TYPES] = {0};

    if ((fp = fopen(FILENAME, "r")) == NULL) {
        printf("ERRORE apertura file di input");
        exit(1);
    }
    fscanf(fp, "%d", &n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < STONES_TYPES; j++) {
            fscanf(fp, "%d", &numStones[j]);
        }
        printf("%d) ", i);
        longestNecklace(stones, numStones);
    }

    fclose(fp);

    return 0;
}

void longestNecklace(char *stones, int *numStones) {
    int *sol, i, k, dim = 0, found = 0;

    for (i = 0; i < STONES_TYPES; i++) {
        dim += stones[i];
    }
    if ((sol = (int *) malloc(dim * sizeof(int))) == NULL) {
        printf("ERROR out of memory\n");
        exit(2);
    }

    for (k = dim - 1; k > 0 && !found; k--) {
        found = longestNecklaceR(0, sol, k, numStones);
    }
    k++;
    if (found) {
        printNecklace(sol, k, stones);
    } else {
        printf("Nessuna soluzione compatibile trovata\n");
    }

    free(sol);

}

int longestNecklaceR(int pos, int *sol, int k, int *numStones) {
    int i;

    // Terminal condition
    if (pos >= k) {
       return 1;
    }

    for (i = 0; i < STONES_TYPES; i++) {
        if (numStones[i] > 0) {
            if (pos > 0) {
                if (sol[pos-1] == Z || sol[pos-1] == T) {
                    if (i != Z && i != R)
                        continue;
                } else if (sol[pos-1] == R || sol[pos-1] == S) {
                    if (i != S && i != T)
                        continue;
                }
            }
            sol[pos] = i;
            numStones[i]--;
            if (longestNecklaceR(pos+1, sol, k, numStones))
                return 1;
            numStones[i]++;
        }
    }

    return 0;
}

int check(int *sol, int k) {
    int i, ok = 1;
    stoneTypes stone; 

    for (i = 0; i < k-1 && ok; i++) {
        stone = sol[i];
        if (stone == Z || stone == T) {
            if (sol[i+1] != Z && sol[i+1] != R)
                ok = 0;
        } else if (stone == R || stone == S) {
            if (sol[i+1] != S && sol[i+1] != T)
                ok = 0;
        }
    }
    return ok;
}

void printNecklace(int *sol, int k, char *stones) {
    int i;

    printf("Migliore soluzione: ");
    for (i = 0; i < k; i++) {
        printf("%c ", stones[sol[i]]);
    }
    printf("\nLunghezza: %d\n", k);
}