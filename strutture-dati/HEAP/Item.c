#include "Item.c"

ItemKey ITEMgetKEY(Item x) {
    return x.key;
}

int KEYcmp(ItemKey k1, ItemKey k2) {
    return k1 - k2;
}