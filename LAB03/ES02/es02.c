/* 
    LAB1 ES01 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI
 */

#include <stdio.h>
#include <stdlib.h>
#define MAXLEN 255

typedef struct amico_s {
    char **playlist;
    int num_proposte;
} Amico;

int readFile(Amico *amici[]);
void showSoluzioni(Amico amici[], char *sol[], int numAmici);
int combinaSoluzione(int pos, Amico amici[], char  *sol[], int numAmici, int cnt);

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
        (*amici)[i].playlist = (char **) malloc((*amici)[i].num_proposte * sizeof(char *));
        if ((*amici)[i].playlist == NULL)
            exit(2);
        for (j = 0; j < (*amici)[i].num_proposte; j++) {
            (*amici)[i].playlist[j] = (char *) malloc(MAXLEN * sizeof(char));
            if ((*amici)[i].playlist[j] ==  NULL)
                exit(2);
            fscanf(fp, "%s", (*amici)[i].playlist[j]);
        }
    }

    fclose(fp);

    return numAmici;
}

void showSoluzioni(Amico amici[], char *sol[], int numAmici) {
    int cnt = 0;

    cnt = combinaSoluzione(0, amici, sol, numAmici, cnt);

    printf("Totale soluzioni: %d", cnt);
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
        sol[pos] = amici[pos].playlist[i];
        cnt = combinaSoluzione(pos+1, amici, sol, numAmici, cnt);
    }

    return cnt;
}