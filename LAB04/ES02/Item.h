/**  
 *  ITEM module interface
 *  Data structure and public available operations declaration
 */

#ifndef ITEM_H
#define ITEM_H

/* Global constants definition */

#define CODE_LEN 5
#define NVARCHAR_LEN 50

/* Type definitions */

typedef struct date_s {
    int day;
    int month;
    int year;
} Date;

typedef struct item_s {
    char *code;
    char *name;
    char *surname;
    Date *date;
    char *address;
    char *city;
    int cap;
} *Item;

typedef struct node_s *link;
typedef struct node_s {
    Item val;
    link next;
} Node;

typedef char* Key;

Item stringToItem(char *str);
Key ItemKey(Item item);
int ItemLt(Key x, Key y);
int ItemEq(Key x, Key y);
int ItemGt(Key x, Key y);
void printItem(FILE *fp, Item x);

/* List operations */

link insertOrderedByCode(Item val, link head);
Item getItemByCode(link head, Key key);
void printList(FILE *fp, link h);

#endif