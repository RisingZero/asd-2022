/* 
    LAB05 ES03 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI
 */

#include <stdio.h>
#include "Inventario.h"
#include "Personaggi.h"

#define INV_FILE "inventario.txt"
#define PG_FILE "pg.txt"

int main(int argc, char const *argv[])
{
    INVcreate();
    leggiInventario(INV_FILE);
    INVprint();
    INVfree();

    LISTcreate();
    leggiPersonaggi(PG_FILE);
    LISTprint();
    LISTfree();

    return 0;
}
