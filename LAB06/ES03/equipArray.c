#include "equipArray.h"

struct equipArray_s {
    int vettEq[EQUIP_SLOT];
    int inUso;
};

equipArray_t equipArray_init() {
    equipArray_t t = (equipArray_t) malloc(sizeof(*t));
    t ->inUso = 0;

    return t;
}

void equipArray_free(equipArray_t equipArray) {
    free(equipArray);
}

int equipArray_inUse(equipArray_t equipArray) {
    return equipArray->inUso;
}

void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray) {
    int i;

    if (equipArray->inUso > 0) {
        for (i = 0; i < equipArray->inUso; i++) {
            invArray_printByIndex(fp, invArray, equipArray->vettEq[i]);
        }
    } else {
        fprintf(fp, "Nessun equipaggiamento\n");
    }
}

void equipArray_update(equipArray_t equipArray, invArray_t invArray) {
    
}

int equipArray_getEquipByIndex(equipArray_t equipArray, int index) {
    return equipArray->vettEq[index];
}