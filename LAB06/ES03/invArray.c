#include "invArray.h"

struct invArray_s {
    inv_t *vettInv;
    int nInv;
    int maxInv;
};

invArray_t invArray_init() {
    invArray_t t = (invArray_t) malloc(sizeof(*t));
    t -> maxInv = 0;
    t -> nInv = 0;

    return t;
}

void invArray_free(invArray_t invArray) {
    free(invArray->vettInv);
    free(invArray);
}

void invArray_read(FILE *fp, invArray_t invArray) {
    int i;

    fscanf(fp, "%d", &(invArray->maxInv));
    invArray->vettInv = (inv_t *) malloc(sizeof(invArray->maxInv * sizeof(inv_t)));

    for (i = 0; i< invArray->maxInv; i++) {
        inv_read(fp, &(invArray->vettInv[i]));
        invArray->nInv++;
    }
}

void invArray_print(FILE *fp, invArray_t invArray) {
    int i;

    inv_header(fp);
    for (i = 0; i < invArray->nInv; i++) {
        inv_print(fp, &(invArray->vettInv[i]));
    }
}

void invArray_printByIndex(FILE *fp, invArray_t invArray, int index) {
    inv_print(fp, &(invArray->vettInv[index]));
}

inv_t *invArray_getByIndex(invArray_t invArray, int index) {
    return &(invArray->vettInv[index]);
}

int invArray_searchByName(invArray_t invArray, char *name) {
    int i;

    for (i = 0; i < invArray->nInv; i++) {
        if (inv_nameCompare(&(invArray->vettInv[i]), name))
            return i;
    }

    // not found
    return -1;
}
