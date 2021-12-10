/* 
    LAB05 ES03 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI
 */

#include <stdio.h>
#include <string.h>
#include "Inventario.h"
#include "Personaggi.h"

#define INV_FILE "inventario.txt"
#define PG_FILE "pg.txt"
#define COMMAND_SIZE 50

typedef enum commands_enum { pg, pg_plus, inv, newpg, delpg, equip, stop, nac } Command;

void availableCommands();
Command readCommand(void);
void handleCommand(Command command);

void handlePGnew();
void handlePGdel();
void handlePGequip();

int main(int argc, char const *argv[])
{
    Command command;

    printf("Allocating memory...\n");
    INVcreate();
    LISTcreate();

    printf("Memory allocated. Loading file data...\n");
    leggiInventario(INV_FILE);
    leggiPersonaggi(PG_FILE);

    do {
        command = readCommand();
        if (command != stop && command != nac)
            handleCommand(command);
    } while(command != stop);

    INVfree();
    LISTfree();
    printf("\nMemory freed!\nBye!\n");

    return 0;
}

void availableCommands() {
    printf(
        "\nComandi disponibili:\n\
        - showpg (Stampa personaggi)\n\
        - showpg+ <XXXX> (Stampa personaggio per codice + valutazione bonus/malus inventario)\n\
        - showinv (Stampa oggetti inventario)\n\
        - newpg (Nuovo personaggio)\n\
        - delpg <XXXX> (Elimina personaggio per codice)\n\
        - equip <XXXX> (Gestione equipaggiamento personaggio selezionato per codice)\n\
        - fine (Terminazione programma)\n"
    );
}

Command readCommand(void) {
    char input[COMMAND_SIZE];
    printf("\nInserisci comando: \n");
    printf("--> ");
    scanf("%s", input);
    fgetc(stdin); //removes /n for following commands

    if (strcmp(input, "showpg") == 0)
        return pg;
    else if (strcmp(input, "showpg+") == 0)
        return pg_plus;
    else if (strcmp(input, "showinv") == 0)
        return inv;
    else if (strcmp(input, "newpg") == 0)
        return newpg;
    else if (strcmp(input, "delpg") == 0)
        return delpg;
    else if (strcmp(input, "equip") == 0)
        return equip;
    else if (strcmp(input, "fine") == 0)
        return stop;
    else {
        printf("ERRORE: comando inserito non esistente!\n");
        availableCommands();
        return nac;
    }
}

void handleCommand(Command command) {
    switch (command){
        case pg:
            LISTprint(0);
            break;
        case pg_plus:
            LISTprint(1);
            break;
        case inv:
            INVprint();
            break;
        case newpg:
            handlePGnew();
            break;
        case delpg:
            handlePGdel();
            break;
        case equip:
            handlePGequip();
            break;
        default:
            break;
    }   
}

void handlePGnew() {
    char input[PG_BUFF_SIZE];
    pg_t pg;

    printf("\nInserisci dato nuovo personaggio nel formato seguente:\n");
    printf("PGXXXX NOME CLASSE HP MP ATK DEF MAG SPR\n");
    printf("--> ");
    fgets(input, PG_BUFF_SIZE, stdin);

    pg = PGscan(input);
    PGinsert(pg);
}

void handlePGdel() {
    pgKey_t inputK;
    pg_t tmp;

    printf("\nInserisci codice personaggio da eliminare: ");
    scanf("%d", &inputK);

    PGdelete(inputK);
}

void handlePGequip() {
    char buffer[PG_BUFF_SIZE];
    int invObjIdx, valid;
    pgKey_t inputK;
    inv_t *obj;
    pg_t pg;

    LISTprint(0);
    printf("\nSelezionare un personaggio di cui si vuole modificare l'equipaggiamento: ");
    scanf("%d", &inputK);

    pg = PGsearch(inputK);
    if (PGisEmpty(pg)) {
        printf("ERRORE, il personaggio indicato non esiste\n");
        return;
    }
    PGprint(pg, 1);
    printf("\nINVENTARIO ATTUALE:\n");
    PGEquipShow(pg);
    
    valid = 0;
    do {
        printf("Operazione da eseguire: (ADD - RM - USE) --> ");
        scanf("%s", buffer);

        if (strcmp(buffer, "ADD") == 0) {
            valid = 1;
            INVprint();
            printf("\nIndica il nome dell'oggetto da aggiungere all'inventario:\n--> ");
            scanf("%s", buffer);
            if ((obj = OGGETTOsearch(buffer)) == NULL) {
                printf("ERRORE, l'oggetto indicato non esiste\n");
                return;
            }            if (!PGequip(pg, obj, add))
                printf("ERRORE, qualcosa è andato storto\n");
        } else if (strcmp(buffer, "RM") == 0) {
            valid = 1;
            INVprint();
            printf("\nIndica il nome dell'oggetto da rimuovere dall'inventario:\n--> ");
            scanf("%s", buffer);
            if ((obj = OGGETTOsearch(buffer)) == NULL) {
                printf("ERRORE, l'oggetto indicato non esiste\n");
                return;
            }
            if (!PGequip(pg, obj, rm))
                printf("ERRORE, qualcosa è andato storto\n");
        } else if (strcmp(buffer, "USE") == 0) {
            valid = 1;
            INVprint();
            printf("\nIndica il nome dell'oggetto da mettere in uso:\n--> ");
            scanf("%s", buffer);
            if ((obj = OGGETTOsearch(buffer)) == NULL) {
                printf("ERRORE, l'oggetto indicato non esiste\n");
                return;
            }            
            if (!PGequip(pg, obj, use))
                printf("ERRORE, qualcosa è andato storto\n");
        } else {
            printf("ERRORE: opzione non disponibile!\n");
        }
    } while (!valid);
}
