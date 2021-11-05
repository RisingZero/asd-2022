/* 
    LAB03 ES02 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXLEN 255

typedef struct amico_s {
    char **song;
    int num_proposte;
} Amico;

int readFile(Amico *amici[]);
void showSoluzioni(Amico amici[], char *sol[], int numAmici);
int combinaSoluzione(int pos, Amico amici[], char *sol[], int numAmici, int cnt);
void freeAll(Amico amici[], char *sol[], int numAmici);

int main(int argc, char const *argv[])
{
    Amico *amici;
    char **sol;
    int i, numAmici;

    numAmici = readFile(&amici);

    sol = (char **) malloc(numAmici * sizeof(char *));
    if (sol == NULL)
        exit(2);
    for (i = 0; i < numAmici; i++) {
        sol[i] = (char *) malloc(MAXLEN * sizeof(char));
        if (sol[i] == NULL)
            exit(2); 
    }

    showSoluzioni(amici, sol, numAmici);
    freeAll(amici, sol, numAmici);
    return 0;
}

int readFile(Amico *amici[]) {
    FILE *fp;
    int numAmici, i, j;

    if((fp = fopen("brani.txt", "r")) == NULL) {
        printf("ERRORE apertura file");
        exit(1);
    }

    fscanf(fp, "%d", &numAmici);
    *amici = (Amico *) malloc(numAmici * sizeof(Amico));

    if (*amici == NULL)
        exit(2);

    for (i = 0; i < numAmici; i++) {
        fscanf(fp, "%d", &((*amici)[i].num_proposte));
        (*amici)[i].song = (char **) malloc((*amici)[i].num_proposte * sizeof(char *));
        if ((*amici)[i].song == NULL)
            exit(2);
        for (j = 0; j < (*amici)[i].num_proposte; j++) {
            (*amici)[i].song[j] = (char *) malloc(MAXLEN * sizeof(char));
            if ((*amici)[i].song[j] ==  NULL)
                exit(2);
            fscanf(fp, "%s", (*amici)[i].song[j]);
        }
    }

    fclose(fp);

    return numAmici;
}

void showSoluzioni(Amico amici[], char *sol[], int numAmici) {
    int cnt = 0;

    cnt = combinaSoluzione(0, amici, sol, numAmici, cnt);

    printf("Totale soluzioni: %d\n", cnt);
}

int combinaSoluzione(int pos, Amico amici[], char  *sol[], int numAmici, int cnt) {
    int i;

    if (pos >= numAmici) {
        printf("Soluzione n.%d\n", cnt);
        for (i = 0; i < numAmici; i++) {
            printf("%d - %s\n", i, sol[i]);
        }
        printf("\n");
        return cnt+1;
    }

    for (i = 0; i < amici[pos].num_proposte; i++) {
        strcpy(sol[pos], amici[pos].song[i]);
        cnt = combinaSoluzione(pos+1, amici, sol, numAmici, cnt);
    }

    return cnt;
}

void freeAll(Amico amici[], char *sol[], int numAmici) {
    int i, j;

    for (i = 0; i < numAmici; i++) {
        for (j = 0; j < amici[i].num_proposte; j++) {
            free(amici[i].song[j]);
        }
        free(amici[i].song);
        free(sol[i]);
    }
    free(amici);
    free(sol);
}
