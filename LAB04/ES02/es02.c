/* 
    LAB04 ES02 S280848 RAINERI ANDREA ANGELO
    ALGORITMI E STRUTTURE DATI  
 */

#include <stdio.h>
#include <string.h>
#include "Item.h"

#define COMMAND_SIZE 50
#define BUFF_SIZE 512

typedef enum commands_enum { node, search, deleteByCode, deleteInDates, print, stop, nac } Command;

void availableCommands();
Command readCommand(void);
void handleCommand(Command command, link *list);
int isFilename(char *str);
void readNode(link *list);
void searchHandler(link list);
void printHandler(link list);
void handleDeleteByCode(link *list);
void handleDeleteInDates(link *list);

int main(int argc, char const *argv[])
{
    link list = NULL;
    Command command;

    do {
        command = readCommand();
        if (command != stop && command != nac)
            handleCommand(command, &list);
    } while(command != stop);

    printf("Arresto programma...\n");
    return 0;
}

void availableCommands() {
    printf(
        "\nComandi disponibili:\n\
        - leggi (Acquisizione nuova anagrafica da tastiera o file)\n\
        - ricerca <AXXXX> (Ricerca anagrafica per codice)\n\
        - del_codice <AXXXX> (Eliminazione anagrafica per codice)\n\
        - del_date gg/mm/aaaa GG/MM/AAAA (Eliminazione anagrafiche tra due date)\n\
        - stampa (Stampa a video o su file delle anagrafiche)\n\
        - fine (Terminazione programma)\n"
    );
}

Command readCommand(void) {
    char input[COMMAND_SIZE];
    printf("\nInserisci comando: \n");
    printf("--> ");
    scanf("%s", input);

    if (strcmp(input, "leggi") == 0)
        return node;
    else if (strcmp(input, "ricerca") == 0)
        return search;
    else if (strcmp(input, "del_codice") == 0)
        return deleteByCode;
    else if (strcmp(input, "del_date") == 0)
        return deleteInDates;
    else if (strcmp(input, "stampa") == 0)
        return print;
    else if (strcmp(input, "fine") == 0)
        return stop;
    else {
        printf("ERRORE: comando inserito non esistente!\n");
        availableCommands();
        return nac;
    }
}

void handleCommand(Command command, link *list) {
    switch (command){
        case node:
            readNode(list);
            break;
        case search:
            searchHandler(*list);
            break;
        case deleteByCode:
            handleDeleteByCode(list);
            break;
        case deleteInDates:
            break;
        case print:
            printHandler(*list);
            break;
        default:
            break;
    }   
}

int isFilename(char *str) {
    size_t lenstr;
    size_t lenext;
    int i = 0, checkExt = 0, checkSpace = 0;
    char *extension = ".txt";

    if (!str)
        return 0;

    while (str[i] != '\0') {
        if (str[i] == ' ')
            checkSpace = 1;
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
        i++;
    }
    
    lenstr = strlen(str);
    lenext = strlen(extension);

    if (lenstr < lenext)
        return 0;
    
    checkExt = strncmp(str + lenstr - lenext, extension, lenext) == 0;
    if (checkExt) {
        if (checkSpace)
            return -1;
        return 1;
    }
    return 0;
}

void readNode(link *list) {
    FILE *fp;
    char input[BUFF_SIZE];
    int nameCheck = 0;
    Item val;

    printf("\n-- ACQUISIZIONE NUOVA ANAGRAFICA --\n");
    printf("Inserisci il nome di un file da cui caricare le anagrafiche o una nuova anagrafica con formattazione:\n");
    printf("<AXXXX> <nome> <cognome> <data di nascita gg/mm/aaaa> <via> <citta'> <cap>\n");
    printf("--> ");

    getchar(); fgets(input, BUFF_SIZE, stdin);
    nameCheck = isFilename(input);
    if (nameCheck == 1) {
        if ((fp = fopen(input, "r")) == NULL) {
            printf("ERRORE apertura file!\n");
            return;
        }
        while (!feof(fp) && (fgets(input, BUFF_SIZE, fp) != NULL)) {
            val = stringToItem(input);
            *list = insertOrderedByCode(val, *list);
        }
        fclose(fp);
    } else if (nameCheck == -1) {
        printf("ERRORE: il nome del file non deve contenere spazi!\n");
        return;
    } else if (nameCheck == 0) {
        val = stringToItem(input);
        *list = insertOrderedByCode(val, *list);
    }
}

void searchHandler(link list) {
    Item res;
    char codekey[CODE_LEN];

    printf("\n-- RICERCA PER CODICE --\n");
    printf("Inserisci il codice dell'anagrafica da ricercare nel formato <AXXXX>\n");
    printf("--> ");

    scanf("%s", codekey);

    res = getItemByCode(list, codekey);
    if (res == NULL) {
        printf("Nessun risultato trovato per codice < %s >\n", codekey);
    } else {
        printf("Risultato ricerca:\n");
        printItem(stdout, res);
    }
}

void printHandler(link list) {
    FILE *fp; fp = NULL;
    int opt;
    char filename[BUFF_SIZE];

    printf("\n-- STAMPA ANAGRAFICHE --\n");
    printf("0 - stampa a video\n1 <nome file> - stampa su file\n");
    printf("--> ");

    if (scanf("%d", &opt) == 1) {
        if (opt == 0) {
            fp = stdout;
        } else if (opt == 1) {
            scanf("%s", filename);
            if (isFilename(filename))
                fp = fopen(filename, "w");
        } else {
            printf("ERRORE comando non valido\n");
            return;
        }

        if (fp == NULL) {
            printf("ERRORE file\n");
            return;
        }

        printList(fp, list);
    } else {
        printf("ERRORE comando non valido\n");
    }
}

void handleDeleteByCode(link *list) {
    Item res;
    char codekey[CODE_LEN];
    
    printf("\n-- ELIMINAZIONE PER CODICE --\n");
    printf("Inserisci il codice dell'anagrafica da eliminare nel formato <AXXXX>\n");
    printf("--> ");

    scanf("%s", codekey);

    res = extractByCode(list, codekey);
    if (res == NULL) {
        printf("Nessun risultato trovato per codice < %s >\n", codekey);
    } else {
        printf("Eliminato il seguente elemento:\n");
        printItem(stdout, res);
    }
}

void handleDeleteInDates(link *list) {
    Item res;
    Date firstDate, secondDate;

    printf("\n-- ELIMINAZIONE ANAGRAFICE TRA DATE DI NASCITA --\n");
    printf("Inserisci i due estremi di date nel formato <gg/mm/aaaa>\n");
    printf("--> ");

    scanf("%d/%d/%d %d/%d/%d",
        firstDate.day, firstDate.month, firstDate.year,
        secondDate.day, secondDate.month, secondDate.year
    );

    while ((res = extractInDates(list, firstDate, secondDate)) != NULL) {
        if (res == NULL) {
            printf("Nessun risultato trovato tra le date indicate\n");
        } else {
            printf("Eliminato il seguente elemento:\n");
            printItem(stdout, res);
        }
    }
}