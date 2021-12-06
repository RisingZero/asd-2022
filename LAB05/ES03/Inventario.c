#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Inventario.h"

tabInv_t *inventario;
char buffer[INV_BUFF_SIZE];

void OGGETTOprint(inv_t obj, int single) {
    if (single) {
        printf("%-50s | %-50s | %4s | %4s | %4s | %4s | %4s | %4s\n",
            "NOME", "TIPO", "HP", "MP", "ATK", "DEF", "MAG", "SPR"
        );
    }

    printf("%-50s | %-50s | %4d | %4d | %4d | %4d | %4d | %4d\n",
        obj.nome, obj.tipo, obj.stat.hp, obj.stat.mp, obj.stat.atk, obj.stat.def, obj.stat.mag, obj.stat.spr
    );
}

static inv_t OGGETTOempty(void) {
    inv_t obj;
    strcpy(obj.nome, "");
    strcpy(obj.tipo, "");
    obj.stat.hp = obj.stat.mp = obj.stat.atk = obj.stat.mag = obj.stat.def = obj.stat.spr = 0;

    return obj;
}

static int INVKEYcompare(invKey_t key1, invKey_t key2) {
    return (strcmp(key1, key2) == 0);
}

static inv_t OGGETTOscan(char *input) {
    inv_t obj;

    sscanf(input, "%s %s %d %d %d %d %d %d", 
        obj.nome,
        obj.tipo,
        &obj.stat.hp,
        &obj.stat.mp,
        &obj.stat.atk,
        &obj.stat.def,
        &obj.stat.mag,
        &obj.stat.spr
    );

    return obj;
}

void INVcreate(void) {
    if ((inventario = (tabInv_t *) malloc(sizeof(tabInv_t))) == NULL) {
        printf("ERRORE memoria esaurita\n");
        exit(2);
    }
}

void INVfree(void) {
    free(inventario->vettInv);
    free(inventario);
}

void leggiInventario(char *filename) {
    FILE *fp;
    inv_t obj;
    int i,N;

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("ERRORE apertura file inventario\n");
        exit(1);
    }
    fscanf(fp, "%d", &N);
    inventario->nInv = N;
    if ((inventario->vettInv = (inv_t *) malloc(N * sizeof(inv_t))) == NULL) {
        printf("ERRORE memoria esaurita\n");
        exit(2);
    }
    getc(fp); // removes \n
    for (i = 0; i < N; i++) {
        fgets(buffer, INV_BUFF_SIZE, fp);
        obj = OGGETTOscan(buffer);
        inventario->vettInv[i] = obj;
    }
    fclose(fp);
}

void INVprint(void) {
    int i;

    printf("%-50s | %-50s | %4s | %4s | %4s | %4s | %4s | %4s\n",
        "NOME", "TIPO", "HP", "MP", "ATK", "DEF", "MAG", "SPR"
    );
    
    for (i = 0; i < inventario->nInv; i++) {
        OGGETTOprint(inventario->vettInv[i], 0);
    }
} 

inv_t OGGETTOsearch(invKey_t key) {
    int i;

    for (i = 0; i < inventario->nInv; i++) {
        if (INVKEYcompare(inventario->vettInv[i].nome, key)) {
            return inventario->vettInv[i];
        }
    }
    return OGGETTOempty();
}
