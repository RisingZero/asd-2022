#ifndef ITEM_MODULE
#define ITEM_MODULE

#define MAX_STR 30

typedef struct item {
    char name[MAX_STR+1];
    char subnet[MAX_STR+1];
} Item;

Item ITEMcreate(char *name, char *subnet);
Item ITEMread();

Item ITEMnull();

void ITEMsort(Item *v, int N);

#endif