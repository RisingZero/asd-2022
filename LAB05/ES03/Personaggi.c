#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Personaggi.h"

static tabPg_t *personaggi;
static char buffer[PG_BUFF_SIZE];

pg_t PGscan(char *input) {
    pg_t pg;
    int i;

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
    pg.equip->inUso = 0;
    for (i = 0; i < MAX_EQUIP; i++) {
        pg.equip->vettEq[i] = NULL;
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

    // Check head
    if (PGKEYcompare(personaggi->headPg->pg.codice, key)) {
        t = personaggi->headPg;
        personaggi->headPg = personaggi->headPg->next;
        PGfree(t);
        personaggi->nPg--;
    }

    // Find in middle
    for (p = personaggi->headPg, t = p->next; t != personaggi->tailPg->next; p = t, t = t->next) {
        if (PGKEYcompare(t->pg.codice, key)) {
            p->next = t->next;
            if (t == personaggi->tailPg) {
                personaggi->tailPg = p;
            }
            PGfree(t);
            personaggi->nPg--;
        }
    }
}

pg_t PGsearch(pgKey_t key){
    link t;

    for (t = personaggi->headPg; t != personaggi->tailPg->next; t = t->next) {
        if (PGKEYcompare(t->pg.codice, key)) {
            return t->pg;
        }
    }

    return PGempty();
}

int PGisEmpty(pg_t pg) {
    return pg.codice == 0;
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

void PGprintPlus(pg_t pg, int single) {
    int _hp, _mp, _atk, _def, _mag, _spr, i;

    _hp = _mp = _atk = _def = _mag = _spr = 0;

    for (i = 0; i < MAX_EQUIP; i++) {
        if (pg.equip->vettEq[i] != NULL) {
            _hp += pg.equip->vettEq[i]->stat.hp;
            _mp += pg.equip->vettEq[i]->stat.mp;
            _atk += pg.equip->vettEq[i]->stat.atk;
            _def += pg.equip->vettEq[i]->stat.def;
            _mag += pg.equip->vettEq[i]->stat.mag;
            _spr += pg.equip->vettEq[i]->stat.spr;
        }
    }

    if (single) {
        printf("%6s | %-50s | %-50s | %4s | %4s | %4s | %4s | %4s | %4s\n",
            "CODICE", "NOME", "CLASSE", "HP", "MP", "ATK", "DEF", "MAG", "SPR"
        );
    }

    printf("PG%04d | %-50s | %-50s | %4d | %4d | %4d | %4d | %4d | %4d\n",
        pg.codice, pg.nome, pg.classe, 
        ((pg.stat.hp + _hp) > 0 ? (pg.stat.hp + _hp) : 0), 
        ((pg.stat.mp + _mp) > 0 ? (pg.stat.mp + _mp) : 0),
        ((pg.stat.atk + _atk) > 0 ? (pg.stat.atk + _atk) : 0),
        ((pg.stat.def + _def) > 0 ? (pg.stat.def + _def) : 0),
        ((pg.stat.mag + _mag) > 0 ? (pg.stat.mag + _mag) : 0),
        ((pg.stat.spr + _spr) > 0 ? (pg.stat.spr + _spr) : 0)
    );
}


static void LISTprintR(link h, int plus) {
    if (h == NULL)
        return;
    if (plus)
        PGprintPlus(h->pg, 0);
    else
        PGprint(h->pg, 0);
    LISTprintR(h->next, plus);
}
void LISTprint(int plus) {
    printf("%6s | %-50s | %-50s | %4s | %4s | %4s | %4s | %4s | %4s\n",
        "CODICE", "NOME", "CLASSE", "HP", "MP", "ATK", "DEF", "MAG", "SPR"
    );
    LISTprintR(personaggi->headPg, plus);
}

int PGequip(pg_t pg, inv_t *obj, equip_t type) {
    int i, isPresent = 0;
    // Check obj presence in pg current inv
    for (i = 0; i < MAX_EQUIP; i++) {
        if (pg.equip->vettEq[i] != NULL && INVKEYcompare(INVKEYget(obj),INVKEYget(pg.equip->vettEq[i]))) {
            isPresent = 1;
        }
    }

    switch (type) {
        case add:
            if (isPresent) {
                printf("ATTENZIONE: l'oggetto indicato è già presente nell'equipaggiamento del personaggio!\n");
                return 0;
            }
            i = 0;
            while (i < MAX_EQUIP && pg.equip->vettEq[i] != NULL) {i++;}
            if (i < MAX_EQUIP) {
                pg.equip->vettEq[i] = obj;
                pg.equip->inUso++;
                return 1;
            } else {
                printf("ATTENZIONE: non ci sono slot disponibili!\n");
                return 0;
            }
            break;
        case rm:
            if (!isPresent) {
                printf("ATTENZIONE: l'oggetto indicato non è presente nell'equipaggiamento del personaggio!\n");
                return 0;
            }
            i = 0;
            while (!INVKEYcompare(INVKEYget(obj),INVKEYget(pg.equip->vettEq[i]))){i++;}
            pg.equip->vettEq[i] = NULL;
            pg.equip->inUso--;
            return 1;
            break;
        default:
            return 0;
            break;
    }
    return 0;
}

void PGEquipShow(pg_t pg) {
    int i;

    if (pg.equip->inUso == 0) {
        printf("Nessun equipaggiamento\n");
        return;
    }
    for (i = 0; i < MAX_EQUIP; i++) {
        if (pg.equip->vettEq[i] != NULL) {
            OGGETTOprint(*(pg.equip->vettEq[i]), i==0);
        }
    }
}