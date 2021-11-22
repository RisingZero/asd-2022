/* 
    LAB04 ES03 S280848 RAINERI ANDREA ANGELO
    ALGORITMI E STRUTTURE DATI  
 */

#include <stdio.h>
#include <stdlib.h>

#define STONES_TYPES 4

typedef enum { Z, R, T, S } stoneTypes;
void longestNecklace(char *stones, int *numStones);
int longestNecklaceR(int pos, int *sol, int k, int *numStones);
int check(int *sol, int k);

int main(int argc, char const *argv[])
{
    char stones[STONES_TYPES] = {'Z', 'R', 'T', 'S'};
    int i, numStones[STONES_TYPES] = {0};

    if (argc != STONES_TYPES+1) {
        printf("Specificare il numero di pietre nell'ordine: Z R T S\n");
        exit(1);
    }

    for (i = 0; i < STONES_TYPES; i++) {
        numStones[i] = atoi(argv[i+1]);
    }

    longestNecklace(stones, numStones);

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
        printf("Migliore soluzione:\n");
        for (i = 0; i < k; i++) {
            printf("%c ", stones[sol[i]]);
        }
        printf("\nLunghezza: %d\n", k);
    } else {
        printf("Nessuna soluzione compatibile trovata\n");
    }

    free(sol);

}

int longestNecklaceR(int pos, int *sol, int k, int *numStones) {
    int i;

    // Terminal condition
    if (pos >= k) {
        if (check(sol, k))
            return 1;
    }

    for (i = 0; i < STONES_TYPES; i++) {
        if (numStones[i] > 0) {
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
        switch (stone) {
            case Z:
                if (sol[i+1] != Z || sol[i+1] != R)
                    ok = 0;
                break;
            case R:
                if (sol[i+1] != S || sol[i+1] != T)
                    ok = 0;
                break;
            case T:
                if (sol[i+1] != Z || sol[i+1] != R)
                    ok = 0;
                break;
            case S:
                if (sol[i+1] != S || sol[i+1] != T)
                    ok = 0;
                break;
            default:
                break;
        }
    }
    return ok;
}
