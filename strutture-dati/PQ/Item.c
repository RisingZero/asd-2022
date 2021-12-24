#include "Item.h"

ItemKey ITEMgetKEY1(Item x) {
    return x.key;
}

ItemPriority ITEMgetPriority(Item x) {
    return x.priority;
}

int PRIORITYcmp(ItemPriority p1, ItemPriority p2) {
    return p1 - p2;
}

int KEYcmp(ItemKey k1, ItemKey k2) {
    return strcmp(k1, k2);
}