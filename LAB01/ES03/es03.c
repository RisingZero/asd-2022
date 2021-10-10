/* 
    LAB1 ES03 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define FILENAME "corse.txt"
#define MAX_LEN 30
#define MAX_NUM 1000

typedef struct corsa_s {
    char codice[MAX_LEN];
    char partenza[MAX_LEN], destinazione[MAX_LEN];
    char data[MAX_LEN];
    char partTime[MAX_LEN], arrivTime[MAX_LEN];
    int ritardo;
} corsa_t;

enum comando_e { r_stampa, r_ord_data, r_ord_codice, r_ord_partenza, r_ord_capolinea, r_ricerca, r_fine };

int readFile(corsa_t corse[]);

enum comando_e leggiComando();
void eseguiComando(enum comando_e comando, corsa_t corse[], int numCorse);

void formattaCorsa(corsa_t corsa, char *str);
void stampaLog(corsa_t corse[], int numCorse);

void ordinaCorse(corsa_t corse[], int numCorse, int (*cmpFunction)(corsa_t corsa1, corsa_t corsa2));
int confrontoData(corsa_t corsa1, corsa_t corsa2);
int confrontoCodice(corsa_t corsa1, corsa_t corsa2);
int confrontoPartenza(corsa_t corsa1, corsa_t corsa2);
int confrontoCapolinea(corsa_t corsa1, corsa_t corsa2);

void eseguiRicerca(corsa_t corse[], int numCorse, int sorted);
void ricercaLineare(corsa_t corse[], int numCorse, char *input);
void ricercaDicotomica(corsa_t corse[], int numCorse, char *input);

int main(int argc, char const *argv[]) {
    enum comando_e comando;
    int numCorse;
    corsa_t corse[MAX_NUM];
    numCorse = readFile(corse);

    do {
        comando = leggiComando();
        if (comando != r_fine)
            eseguiComando(comando, corse, numCorse);
    } while (comando != r_fine);

    return 0;
}

int readFile(corsa_t corse[]) {
    FILE *fp;
    int i;
    int numCorse;

    fp = fopen(FILENAME, "r");
    fscanf(fp, "%d", &numCorse);

    for (i = 0; i < numCorse; i++) {
        fscanf(fp, "%s %s %s %s %s %s %d", 
            corse[i].codice,
            corse[i].partenza,
            corse[i].destinazione,
            corse[i].data,
            corse[i].partTime,
            corse[i].arrivTime,
            &corse[i].ritardo );
    }

    fclose(fp);

    return numCorse;
}

// Command reader
enum comando_e leggiComando() {
    int comandoOK = 0;
    char input[MAX_LEN];

    do {
        printf("Indicare un comando da eseguire: ");
        scanf("%s", input);

        comandoOK = 1;
        if (strcmp(input, "stampa") == 0) {
            return r_stampa;
        } else if (strcmp(input, "ord_data") == 0) {
            return r_ord_data;
        } else if (strcmp(input, "ord_codice") == 0) {
            return r_ord_codice;
        } else if (strcmp(input, "ord_partenza") == 0) {
            return r_ord_partenza;
        } else if (strcmp(input, "ord_capolinea") == 0) {
            return r_ord_capolinea;
        } else if (strcmp(input, "ricerca") == 0) {
            return r_ricerca;
        } else if (strcmp(input, "fine") == 0) {
            return r_fine;
        } else {
            printf("ERRORE: il comando inserito non esiste!\n");
            comandoOK = 0;
        }

    } while (!comandoOK);
}

// Command handler
void eseguiComando(enum comando_e comando, corsa_t corse[], int numCorse) {
    static int sorted = 0;

    switch (comando) {
        case r_stampa:
            stampaLog(corse, numCorse);
            break;
        case r_ord_data:
            ordinaCorse(corse, numCorse, confrontoData);
            sorted = 0;
            break;
        case r_ord_codice:
            ordinaCorse(corse, numCorse, confrontoCodice);
            sorted = 0;
            break;
        case r_ord_partenza:
            ordinaCorse(corse, numCorse, confrontoPartenza);
            sorted = 1;
            break;
        case r_ord_capolinea:
            ordinaCorse(corse, numCorse, confrontoCapolinea);
            sorted = 0;
            break;
        case r_ricerca:
            eseguiRicerca(corse, numCorse, sorted);
            break;
        default:
            break;
    }
}

void formattaCorsa(corsa_t corsa, char *str) {
    sprintf(str, "%s %s %s %s %s %s %d\n", 
        corsa.codice,
        corsa.partenza,
        corsa.destinazione,
        corsa.data,
        corsa.partTime,
        corsa.arrivTime,
        corsa.ritardo
    );
}

void stampaLog(corsa_t corse[], int numCorse) {
    FILE *fp;
    int i, opt, validInput = 0;
    char filename[MAX_LEN], str[MAX_LEN*10];

    do {
        printf("Stampa a video (0) o su file (1 <nome_file>): ");
        scanf("%d", &opt);

        if (opt == 0) {
            validInput = 1;
        } else if ((opt == 1) && scanf("%s", filename) == 1) {
            validInput = 1;
        } else {
            validInput = 0;
        }
    } while (!validInput);
    
    if (opt == 0) {
        for (i = 0; i < numCorse; i++) {
            formattaCorsa(corse[i], str);
            printf("%s", str);
        }
    } else {
        if ((fp = fopen(filename, "w")) == NULL) {
            printf("ERRORE creazione file");
            exit(1);
        }

        for (i = 0; i < numCorse; i++) {
            formattaCorsa(corse[i], str);
            fprintf(fp, "%s", str);
        }

        fclose(fp);
    }
    
}

// Insertion Sort implementation
void ordinaCorse(corsa_t corse[], int numCorse, int (*cmpFunction)(corsa_t corsa1, corsa_t corsa2)) {
    int i, j, l=0, r=numCorse-1;
    corsa_t x;

    for (i = l+1; i <= r; i++) {
        x = corse[i];
        for (j = i-1; j >= l && (*cmpFunction)(x, corse[j]) < 0; j--) {
            corse[j+1] = corse[j];
        }
        corse[j+1] = x;
    }
}

/* 
    Compare functions for sorting
 */
// Sort for date/departure time
int confrontoData(corsa_t corsa1, corsa_t corsa2) {
    int res;
    if ((res = strcmp(corsa1.data, corsa2.data)) == 0) {
        return strcmp(corsa1.partTime, corsa2.partTime);
    } else {
        return res;
    }
}

// Sort for code
int confrontoCodice(corsa_t corsa1, corsa_t corsa2) {
    return strcmp(corsa1.codice, corsa2.codice);
}

// Sort for departure
int confrontoPartenza(corsa_t corsa1, corsa_t corsa2) {
    return strcmp(corsa1.partenza, corsa2.partenza);
}

// Sort for arrival
int confrontoCapolinea(corsa_t corsa1, corsa_t corsa2) {
    return strcmp(corsa1.destinazione, corsa2.destinazione);
}

// Search handler
void eseguiRicerca(corsa_t corse[], int numCorse, int sorted) {

    char input[MAX_LEN];
    int idx;

    printf("Inserire nome (o parziale) della stazione di partenza: ");
    scanf("%s", input);

    if (!sorted) {
        printf("I dati non risultano ordinati, effettuo una ricerca lineare\n");
        ricercaLineare(corse, numCorse, input);
    } else {
        printf("I dati risultano ordinati, effettuo una ricerca dicotomica\n");
        ricercaDicotomica(corse, numCorse, input);
    }
}

void ricercaLineare(corsa_t corse[], int numCorse, char *input) {
    int i, found = 0;
    char str[MAX_LEN*10];
    for (i = 0; i < numCorse; i++) {
        if (strncmp(corse[i].partenza, input, __min(strlen(input), strlen(corse[i].partenza))) == 0) {
            formattaCorsa(corse[i], str);
            printf("Posizione: %d -- %s", i, str);
            found = 1;
        }
    }
    if (!found) {
        printf("Nessun dato trovato\n");       
    }
}

void ricercaDicotomica(corsa_t corse[], int numCorse, char *input) {
    int found = 0, idx, l=0, r=numCorse-1, cmp;
    int i;
    char str[MAX_LEN*10];

    while (!found && (l <= r)) {
        idx = (r+l)/2;
        cmp = strncmp(corse[idx].partenza, input, __min(strlen(input), strlen(corse[idx].partenza)));
        if (cmp == 0) {
            found = 1;
        } else if (cmp > 0) {
            r = idx - 1;
        } else {
            l = idx + 1;
        }
    }
    
    if (found) {
        formattaCorsa(corse[idx], str);
        printf("Posizione: %d -- %s", idx, str);

        i = idx + 1;
        while((i < numCorse) && (strncmp(corse[i].partenza, input, __min(strlen(input), strlen(corse[i].partenza))) == 0)) {
            formattaCorsa(corse[i], str);
            printf("Posizione: %d -- %s", i, str);
            i++;
        }
        i = idx - 1;
        while((i >= 0) && (strncmp(corse[i].partenza, input, __min(strlen(input), strlen(corse[i].partenza))) == 0)) {
            formattaCorsa(corse[i], str);
            printf("Posizione: %d -- %s", i, str);
            i--;
        }
    } else {
        printf("Nessun dato trovato\n");
    }
    
}