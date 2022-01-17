#include "Item.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Item ITEMcreate(char *name, char *subnet) {
    Item t;

    strcpy(t.name, name);
    strcpy(t.subnet, subnet);

    return t;
}

Item ITEMread() {
    Item item;

    scanf("%s %s", item.name, item.subnet);

    return item;
}

Item ITEMnull() {
    Item t;
    strcpy(t.name, "");
    strcpy(t.subnet, "");

    return t;
}

static int compareItem(const void *a, const void *b) {
    return strcmp((*(Item*)a).name, (*(Item*)b).name);
}

void ITEMsort(Item *v, int N) {
    qsort(v, N, sizeof(Item), compareItem);
}