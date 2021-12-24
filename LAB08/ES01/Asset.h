/* 
    Asset ADT module
 */
#ifndef ASSET_MODULE
#define ASSET_MODULE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Exrate.h"
#include "ExrateBST.h"

#define CODE_LEN 20

typedef struct asset_ *Asset;
typedef char* AssetKey;

Asset Asset_create();
void Asset_free(Asset asset);

Asset Asset_scan(FILE *fp);
void Asset_printShort(FILE *fp, Asset asset);

AssetKey Asset_getKey(Asset t);
int Asset_KeyCompare(AssetKey k1, AssetKey k2);

ExrateBST Asset_getExrates(Asset t);

#endif