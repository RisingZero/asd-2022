/* 
    Exrate module
 */
#ifndef EXRATE_MODULE
#define EXRATE_MODULE

#include <stdio.h>
#include <stdlib.h>

#include "Datetime.h"

typedef struct exrate_ {
    Datetime date; // Date of the exchange rate
    int q; // Current exchange rate of an asset
    int n; // Total number of transactions of the day
} Exrate;

Exrate Exrate_read(FILE *fp);

Exrate Exrate_null();

#endif