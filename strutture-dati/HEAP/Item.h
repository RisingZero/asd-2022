#ifndef ITEM_MODULE
#define ITEM_MODULE

#define MAX_LEN 25

typedef struct item_s {
    char s[MAX_LEN];
    int key;
} Item;

typedef int ItemKey;

ItemKey ITEMgetKEY(Item x);
int KEYcmp(ItemKey k1, ItemKey k2);

#endif