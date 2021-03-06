/**  
 *  PERSONAGGI module interface
 *  Data structure and public available operations declaration
 *
*/

#ifndef PERSONAGGI_H
#define PERSONAGGI_H

#include "Inventario.h"

/* Global constants definition */

#define PG_BUFF_SIZE 1024
#define MAX_PG_STRLEN 50
#define MAX_EQUIP 8

/* Type definitions */

typedef struct {
    int inUso;
    inv_t *vettEq[MAX_EQUIP];
} tabEquip_t;

typedef struct {
    int codice;
    char nome[MAX_PG_STRLEN+1];
    char classe[MAX_PG_STRLEN+1];
    tabEquip_t *equip;
    stat_t stat;
} pg_t;

typedef int pgKey_t;

typedef struct node_s nodoPg_t, *link;
struct node_s {
    pg_t pg;
    link next;
};

typedef struct {
    link headPg;
    link tailPg;
    int nPg;
} tabPg_t;

typedef enum { add, rm, use } equip_t;

/* Function prototypes */

void LISTcreate(void);
void LISTfree(void);
void leggiPersonaggi(char *filename);
pg_t PGscan(char *input);
void PGinsert(pg_t pg);
void PGdelete(pgKey_t key);
pg_t PGsearch(pgKey_t key);
int PGisEmpty(pg_t pg);
void PGprint(pg_t pg, int single);
void LISTprint(int plus);
int PGequip(pg_t pg, inv_t *obj, equip_t type);
void PGEquipShow(pg_t pg);

#endif