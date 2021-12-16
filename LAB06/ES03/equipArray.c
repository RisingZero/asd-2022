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
    char buffer[LEN];
    int valid = 0, obj, i, j;

    do {
        printf("Operazione da eseguire: (ADD - RM) --> ");
        scanf("%s", buffer);

        if (strcmp(buffer, "ADD") == 0) {
            valid = 1;
            if (equipArray->inUso == EQUIP_SLOT) {
                printf("Attenzione, non ci sono slot disponibili\n");
                return;
            }
            printf("\nIndica il nome dell'oggetto da aggiungere all'inventario:\n--> ");
            scanf("%s", buffer);
            if ((obj = invArray_searchByName(invArray, buffer)) == -1) {
                printf("ERRORE, l'oggetto indicato non esiste\n");
                return;
            }
            equipArray->vettEq[equipArray->inUso++] = obj;
        } else if (strcmp(buffer, "RM") == 0) {
            valid = 1;
            if (equipArray->inUso == 0) {
                printf("Attenzione, non ci sono slot occupati\n");
                return;
            }
            printf("\nIndica il nome dell'oggetto da rimuovere dall'inventario:\n--> ");
            scanf("%s", buffer);
            if ((obj = invArray_searchByName(invArray, buffer)) == -1) {
                printf("ERRORE, l'oggetto indicato non esiste\n");
                return;
            }
            
            for (i = 0; i < equipArray->inUso; i++) {
                if (equipArray_getEquipByIndex(equipArray, i) == obj) {
                    for (j = i; j < equipArray->inUso - 1; j++) {
                        equipArray->vettEq[j] = equipArray->vettEq[j+1];
                    }
                    equipArray->vettEq[equipArray->inUso] = -1;
                    equipArray->inUso--;
                    return;
                }
            }
            printf("Oggetto non trovato nell'inventario\n");
        } else {
            printf("ERRORE, operazione non esistente\n");
        }
    } while (!valid);
}

int equipArray_getEquipByIndex(equipArray_t equipArray, int index) {
    return equipArray->vettEq[index];
}