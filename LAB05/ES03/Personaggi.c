#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Personaggi.h"

tabPg_t *personaggi;
char buffer[PG_BUFF_SIZE];

static pg_t PGscan(char *input) {
    pg_t pg;

    sscanf(input, "PG%d %s %s %d %d %d %d %d %d",
        &pg.codice,
        pg.nome,
        pg.classe,
        &pg.stat.hp,
        &pg.stat.mp,
        &pg.stat.atk,
        &pg.stat.def,
        &pg.stat.mag,
        &pg.stat.spr
    );

    if ((pg.equip = (tabEquip_t*) malloc(sizeof(tabEquip_t))) == NULL) {
        printf("ERRORE memoria esaurita");
        exit(2);
    }

    return pg;
}

static link LISTnewNode(pg_t pg, link next) {
    link node;
    if ((node = (link) malloc(sizeof(nodoPg_t))) == NULL) {
        printf("ERRORE memoria esaurita");
        exit(2);
    }
    
    node->pg = pg;
    node->next = next;

    return node;
}

static pg_t PGempty(void) {
    pg_t pg;
    
    pg.codice = 0;
    strcpy(pg.nome, "");
    strcpy(pg.classe, "");
    pg.stat.hp = pg.stat.mp = pg.stat.atk = pg.stat.mag = pg.stat.def = pg.stat.spr = 0;
    pg.equip = NULL;

    return pg;
}

static void PGfree(link node) {
    free(node->pg.equip);
    free(node);
}

static int PGKEYcompare(pgKey_t key1, pgKey_t key2) {
    return key1 == key2;
}

void LISTcreate(void) {
    if ((personaggi = (tabPg_t*) malloc(sizeof(tabPg_t))) == NULL) {
        printf("ERRORE memoria esaurita\n");
        exit(2);
    }
    personaggi->headPg = NULL;
    personaggi->tailPg = NULL;
    personaggi->nPg = 0;
}

static void LISTfreeR(link node) {
    if (node->next == NULL) {
        return;
    }

    LISTfreeR(node->next);
    PGfree(node);
}
void LISTfree(void) {
    link t, p;
    pg_t x;

    LISTfreeR(personaggi->headPg);
    free(personaggi);
}

void leggiPersonaggi(char *filename){
    FILE *fp;
    pg_t pg;
    int N;

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("ERRORE apertura file personaggi");
        exit(1);
    }

    N = 0;
    while (!feof(fp) && (fgets(buffer, PG_BUFF_SIZE, fp) != NULL)) {
        pg = PGscan(buffer);
        PGinsert(pg);
    }
    fclose(fp);

}

void PGinsert(pg_t pg){
    link node;

    node = LISTnewNode(pg, personaggi->headPg);
    personaggi->headPg = node; personaggi->nPg++;
    if (personaggi->nPg == 1)
        personaggi->tailPg = node;
}

void PGdelete(pgKey_t key){
    link t, p;

    for (t = personaggi->headPg, p = NULL; t->next == NULL; p = t, t = t-> next) {
        if (PGKEYcompare(t->pg.codice, key)) {
            p->next = t->next;
            PGfree(t);
        }
    }
}

pg_t PGsearch(pgKey_t key){
    link t;

    for (t = personaggi->headPg; t->next != NULL; t = t->next) {
        if (PGKEYcompare(t->pg.codice, key)) {
            return t->pg;
        }
    }

    return PGempty();
}

void PGprint(pg_t pg, int single) {
    if (single) {
        printf("%6s | %-50s | %-50s | %4s | %4s | %4s | %4s | %4s | %4s\n",
            "CODICE", "NOME", "CLASSE", "HP", "MP", "ATK", "DEF", "MAG", "SPR"
        );
    }

    printf("PG%04d | %-50s | %-50s | %4d | %4d | %4d | %4d | %4d | %4d\n",
        pg.codice, pg.nome, pg.classe, pg.stat.hp, pg.stat.mp, pg.stat.atk, pg.stat.def, pg.stat.mag, pg.stat.spr
    );
}

static void LISTprintR(link h) {
    if (h == NULL)
        return;
    PGprint(h->pg, 0);
    LISTprintR(h->next);
}
void LISTprint() {
    printf("%6s | %-50s | %-50s | %4s | %4s | %4s | %4s | %4s | %4s\n",
        "CODICE", "NOME", "CLASSE", "HP", "MP", "ATK", "DEF", "MAG", "SPR"
    );
    LISTprintR(personaggi->headPg);
}