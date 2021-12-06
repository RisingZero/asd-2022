/**  
 *  INVENTARIO module interface
 *  Data structure and public available operations declaration
 *
*/

#ifndef INVENTARIO_H
#define INVENTARIO_H

/* Global constants definition */

#define INV_BUFF_SIZE 1024
#define MAX_INV_STRLEN 50

/* Type definitions */

typedef struct {
    int hp, mp, atk, def, mag, spr;
} stat_t;

typedef struct {
    char nome[MAX_INV_STRLEN+1];
    char tipo[MAX_INV_STRLEN+1];
    stat_t stat;
} inv_t;

typedef char* invKey_t;

typedef struct {
    inv_t *vettInv;
    int nInv;
} tabInv_t;

/* Function prototypes */

void INVcreate(void);
void INVfree(void);
void leggiInventario(char *filename);
void INVprint(void);
inv_t OGGETTOsearch(invKey_t key);
void OGGETTOprint(inv_t obj, int single);

#endif