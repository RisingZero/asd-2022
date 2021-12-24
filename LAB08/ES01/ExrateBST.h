/* 
    ExrateBST ADT module
 */
#ifndef EXRATEBST_MODULE
#define EXRATEBST_MODULE

#include <stdio.h>
#include <stdlib.h>

#include "Exrate.h"
#include "Datetime.h"

typedef struct exrate_bst_ *ExrateBST;

ExrateBST ExrateBST_init();
void ExrateBST_free(ExrateBST bst);

void ExrateBST_insert(ExrateBST bst, Exrate exrate);
void ExrateBST_merge(ExrateBST dest, ExrateBST src);

void ExrateBST_balance(ExrateBST bst);

#endif