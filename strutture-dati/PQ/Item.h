#ifndef ITEM_MODULE
#define ITEM_MODULE

#include <string.h>

#define MAX_LEN 25

typedef struct item_s {
    char key[MAX_LEN];
    int priority;
} Item;

typedef char *ItemKey;
typedef int ItemPriority;

ItemKey ITEMgetKEY1(Item x);
ItemPriority ITEMgetPriority(Item x);
int KEYcmp(ItemKey k1, ItemKey k2);
int PRIORITYcmp(ItemPriority p1, ItemPriority p2);

#endif