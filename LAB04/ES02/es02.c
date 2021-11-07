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
void printHandler(link list);

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
        "Comandi disponibili:\n\
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
    printf("Inserisci comando: \n");
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
            break;
        case deleteByCode:
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
    int i = 0;
    char *extension = ".txt";

    if (!str)
        return 0;

    while (str[i] != '\0') {
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
    
    return strncmp(str + lenstr - lenext, extension, lenext) == 0;
}

void readNode(link *list) {
    FILE *fp;
    char input[BUFF_SIZE];
    Item val;

    printf("-- ACQUISIZIONE NUOVA ANAGRAFICA --\n");
    printf("Inserisci il nome di un file da cui caricare le anagrafiche o una nuova anagrafica con formattazione:\n");
    printf("<AXXXX> <nome> <cognome> <data di nascita gg/mm/aaaa> <via> <citta'> <cap>\n");
    printf("--> ");

    getchar(); fgets(input, BUFF_SIZE, stdin);

    if (isFilename(input)) {
        if ((fp = fopen(input, "r")) == NULL) {
            printf("ERRORE apertura file!\n");
            return;
        }
        while (!feof(fp) && (fgets(input, BUFF_SIZE, fp) != NULL)) {
            val = stringToItem(input);
            *list = insertOrderedByCode(val, *list);
        }
        fclose(fp);
    } else {
        val = stringToItem(input);
        *list = insertOrderedByCode(val, *list);
    }
}

void printHandler(link list) {
    FILE *fp; fp = NULL;
    int opt;
    char filename[BUFF_SIZE];

    printf("-- STAMPA ANAGRAFICHE --\n");
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
