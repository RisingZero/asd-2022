/* 
    AssetList ADT module
 */
#ifndef ASSETLIST_MODULE
#define ASSETLIST_MODULE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Asset.h"

typedef struct asset_list_ *AssetList;

AssetList AssetList_init();
void AssetList_free(AssetList assetList);

void AssetList_show(FILE *fp, AssetList assetList);
int AssetList_isEmpty(AssetList assetList);

void AssetList_insert(AssetList assetList, Asset asset);

Asset *AssetList_getByIndex(AssetList assetList, int index);
#endif