#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "GRAPH.h"

#define MAX_LEN 100
#define BUFF_SIZE 1024

typedef enum bool { false, true } Boolean;
typedef enum command { stop, ladj, display, vadjacency } Command;

Graph readGraph(char *filename);

void showCommands();
Command readCommand();
void handleCommand(Command c, void *context);

int main(int argc, char const *argv[])
{
    char filename[MAX_LEN];
    Command c;
    Graph G;
    int V;
    if (argc > 1) {
        strcpy(filename, argv[1]);
    } else {
        printf("ERRORE: Invalid arguments, please specify a filename");
        return 1;
    }

    G = readGraph(filename);

    while ((c = readCommand()) != stop) {
        handleCommand(c, &G);
    }

    GRAPHfree(G);

    return 0;
}

Graph readGraph(char *filename) {
    FILE *fin;
    int x = 0, i, wt;
    char buffer[BUFF_SIZE];
    Graph G;

    char name1[MAX_STR], name2[MAX_STR], subnet1[MAX_STR], subnet2[MAX_STR];

    if ((fin = fopen(filename, "r")) == NULL) {
        printf("ERROR opening input file");
        exit(1);
    }
    while(fgets(buffer, BUFF_SIZE, fin) != NULL) {
        x++;
    }
    fclose(fin);

    G = GRAPHinit(x*x);

    if ((fin = fopen(filename, "r")) == NULL) {
        printf("ERROR opening input file");
        exit(1);
    }
    for (i = 0; i < x; i++) {
        fscanf(fin, "%s %s %s %s %d", name1, name2, subnet1, subnet2, &wt);
        GRAPHinsertE(G, name1, name2, subnet1, subnet2, wt);
    }
    fclose(fin);

    return G;
}

void showCommands() {
    printf("0) Fine\n");
    printf("1) Costruisci lista di adiacenza\n");
    printf("2) Stampa vertici e archi in ordine alfabetico\n");
    printf("3) Controlla adiacenza tra 3 vertici\n");
    printf("-> ");
}

Command readCommand() {
    Command c;
    Boolean valid = false;
    int input;

    while (!valid) {
        showCommands();
        scanf("%d", &input);
        valid = true;
        if (input == 0) {
            return stop;
        } else if (input == 1) {
            return ladj;
        } else if (input == 2) {
            return display;
        } else if (input == 3) {
            return vadjacency;
        } else {
            valid = false;
            printf("Comando non valido\n");
        }
    }
}

void handleCommand(Command c, void *context) {
    switch(c) {
        case ladj:
            GRAPHbuildLadj(*(Graph *)context);
            break;
        case display:
            GRAPHdisplayVertexEdgesOrdered(*(Graph *)context);
            break;
        case vadjacency:
            GRAPHthreeVAdjacency(*(Graph *)context);
            break;
        default:
            break;
    }
}
