/* 
    LAB1 ES02 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI
 */

#include <stdio.h>
#include <string.h>
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

enum comando_e { r_date, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot, r_fine };

int readFile(corsa_t corse[]);
enum comando_e leggiComando();
void selezionaDati(corsa_t corse[], int numCorse, enum comando_e comando);
void stampaCorsa(corsa_t corsa);

void getTripsBetweenDate(corsa_t corse[], int numCorse);
void getTripsByStop(corsa_t corse[], int numCorse);
void getTripsByTerminal(corsa_t corse[], int numCorse);
void getLateTripsBetweenDate(corsa_t corse[], int numCorse);
void getDelayByTripCode(corsa_t corse[], int numCorse);

int main(int argc, char const *argv[])
{
    enum comando_e comando;
    int numCorse;
    corsa_t corse[MAX_NUM];
    numCorse = readFile(corse);

    do {
        comando = leggiComando();
        if (comando != r_fine)
            selezionaDati(corse, numCorse,  comando);
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

enum comando_e leggiComando() {
    int comandoOK = 0;
    char input[MAX_LEN];

    do {
        printf("Indicare un comando da eseguire: ");
        scanf("%s", input);

        comandoOK = 1;
        if (strcmp(input, "date") == 0) {
            return r_date;
        } else if (strcmp(input, "partenza") == 0) {
            return r_partenza;
        } else if (strcmp(input, "capolinea") == 0) {
            return r_capolinea;
        } else if (strcmp(input, "ritardo") == 0) {
            return r_ritardo;
        } else if (strcmp(input, "ritardo_tot") == 0) {
            return r_ritardo_tot;
        } else if (strcmp(input, "fine") == 0) {
            return r_fine;
        } else {
            printf("ERRORE: il comando inserito non esiste!\n");
            comandoOK = 0;
        }

    } while (!comandoOK);
}

void selezionaDati(corsa_t corse[], int numCorse, enum comando_e comando) {

    switch (comando) {
        case r_date:
            getTripsBetweenDate(corse, numCorse);
            break;
        case r_partenza:
            getTripsByStop(corse, numCorse);
            break;
        case r_capolinea:
            getTripsByTerminal(corse, numCorse);
            break;
        case r_ritardo:
            getLateTripsBetweenDate(corse, numCorse);
            break;
        case r_ritardo_tot:
            getDelayByTripCode(corse, numCorse);
            break;
        default:
            break;
    }
}

void getTripsBetweenDate(corsa_t corse[], int numCorse) {
    int i;
    char data1[MAX_LEN], data2[MAX_LEN];

    if (scanf("%s %s", data1, data2) == 2) {
        for (i = 0; i < numCorse; i++) {
            if (strcmp(corse[i].data, data1) > 0 && strcmp(corse[i].data, data2) < 0)
                stampaCorsa(corse[i]);
        }
    } else {
        printf("ERRORE: il comando non è stato indicato correttamente");
    }
}

void getTripsByStop(corsa_t corse[], int numCorse) {
    int i;
    char stopName[MAX_LEN];

    if (scanf("%s", stopName) == 1) {
        for (i = 0; i < numCorse; i++) {
            if (strcmp(corse[i].partenza, stopName) == 0)
                stampaCorsa(corse[i]);
        }
    } else {
        printf("ERRORE: il comando non è stato indicato correttamente");
    }
    
}

void getTripsByTerminal(corsa_t corse[], int numCorse) {
    int i;
    char terminalName[MAX_LEN];

    if (scanf("%s", terminalName) == 1) {
        for (i = 0; i < numCorse; i++) {
            if (strcmp(corse[i].destinazione, terminalName) == 0)
                stampaCorsa(corse[i]);
        }
    } else {
        printf("ERRORE: il comando non è stato indicato correttamente");
    }
}

void getLateTripsBetweenDate(corsa_t corse[], int numCorse) {
    int i;
    char data1[MAX_LEN], data2[MAX_LEN];

    if (scanf("%s %s", data1, data2) == 2) {
        for (i = 0; i < numCorse; i++) {
            if (strcmp(corse[i].data, data1) > 0 && strcmp(corse[i].data, data2) < 0 && corse[i].ritardo > 0) 
                stampaCorsa(corse[i]);
        }
    } else {
        printf("ERRORE: il comando non è stato indicato correttamente");
    }
}

void getDelayByTripCode(corsa_t corse[], int numCorse) {
    int i;
    int totDelay = 0;
    char tripCode[MAX_LEN];

    if (scanf("%s", tripCode) == 1) {
        for (i = 0; i < numCorse; i++) {
            if (strcmp(corse[i].codice, tripCode) == 0) {
                stampaCorsa(corse[i]);
                totDelay += corse[i].ritardo;
            }
        }
        printf("Ritardo totale accumulato: %d\n", totDelay);
    } else {
        printf("ERRORE: il comando non è stato indicato correttamente\n");
    }
    
}

void stampaCorsa(corsa_t corsa) {
    printf("%s %s %s %s %s %s %d\n", 
        corsa.codice,
        corsa.partenza,
        corsa.destinazione,
        corsa.data,
        corsa.partTime,
        corsa.arrivTime,
        corsa.ritardo
    );
}