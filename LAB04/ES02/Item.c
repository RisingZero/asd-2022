/**
 *  ITEM module implementation
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Item.h"

/** 
 * * Creates new node and returns pointer to the node
 * @param Item Data of the new node
 * @param link Pointer to the next node
 * @return link Pointer to the new node
 */
static link newNode(Item val, link next) {
    link t;
    t = (link) malloc(sizeof(Node));
    if (t == NULL) {
        printf("ERROR: out of memory");
        exit(2);
    }
    t->val = val;
    t->next = next;

    return t;
}

Item stringToItem(char *str) {
    Item x;
    char code[CODE_LEN+1];
    char name[NVARCHAR_LEN+1];
    char surname[NVARCHAR_LEN+1];
    Date date;
    char address[NVARCHAR_LEN+1];
    char city[NVARCHAR_LEN+1];
    int cap;

    sscanf(str, "%s %s %s %d/%d/%d %s %s %d",
        code,
        name,
        surname,
        &(date.day),
        &(date.month),
        &(date.year),
        address,
        city,
        &cap
    );

    if ((x = (Item) malloc(sizeof(struct item_s))) == NULL) {
        printf("ERROR: out of memory");
        exit(2); 
    }

    x->code = strdup(code);
    x->name = strdup(name);
    x->surname = strdup(surname);
    if ((x->date = (Date *) malloc(sizeof(Date))) == NULL) {
        printf("ERROR: out of memory");
        exit(2);
    }
    x->date->day = date.day;
    x->date->month = date.month;
    x->date->year = date.year;
    x->address = strdup(address);
    x->city = strdup(city);
    x->cap = cap;

    return x;
}

Key ItemKey(Item val) {
    return val->code;
}

int ItemLt(Key x, Key y) {
    return (strcmp(x, y) ==  -1);
}

int ItemEq(Key x, Key y) {
    return (strcmp(x, y) == 0);
}

int ItemGt(Key x, Key y) {
    return (strcmp(x, y) == 1);
}

void printItem(FILE *fp, Item x) {
    fprintf(fp, "%s %s %s %02d/%02d/%04d %s %s %d\n",
        x->code,
        x->name,
        x->surname,
        x->date->day,
        x->date->month,
        x->date->year,
        x->address,
        x->city,
        x->cap
    );
}

link insertOrderedByCode(Item val, link head) {
    link x, p;

    if (head == NULL || ItemGt(ItemKey(head->val), ItemKey(val)))
        return newNode(val, head);

    for (p = head, x = head->next; x != NULL && ItemGt(ItemKey(val), ItemKey(x->val)); p = x, x = x->next);
    p->next = newNode(val, x);

    return head;
}

Item getItemByCode(link head, Key key) {
    link t;

    for (t = head; t != NULL; t = t->next) {
        if (ItemEq(t->val->code, key))
            return t->val;
    }

    return NULL;
}

void printList(FILE *fp, link h) {
    if (h == NULL)
        return;

    printItem(fp, h->val);
    printList(fp, h->next);
}
