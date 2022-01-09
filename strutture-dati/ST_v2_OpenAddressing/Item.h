#ifndef ITEM_MODULE
#define ITEM_MODULE

#define MAX_LEN 25

#include <string.h>

typedef struct item_s {
    char s[MAX_LEN];
    int key;
} Item;

typedef char* Key;

Key ITEMgetKEY(Item x);
int KEYcmp(Key k1, Key k2);

Item ITEMsetNull();

#endif