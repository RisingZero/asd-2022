#include "Item.h"

Key ITEMgetKEY(Item x) {
    return x.s;
}

int KEYcmp(Key k1, Key k2) {
    return strcmp(k1, k2);
}

Item ITEMsetNull() {
    Item t;
    strcpy(t.s, "");
    t.key = 0;

    return t;
}