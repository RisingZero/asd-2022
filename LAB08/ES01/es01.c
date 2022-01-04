/* 
    RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI A.A.2021-22
 */
#include <stdio.h>
#include <string.h>

#include "AssetList.h"
#include "Asset.h"
#include "Exrate.h"
#include "ExrateBST.h"

#define MAX_LEN 512

typedef enum commands_ { 
    end,
    readf,
    asset,
    singleExrate,
    minmaxExrateInterval,
    minmaxExrate,
    balance
} Command;

Command readCommand(void);
void showCommands();
void handleCommand(Command command, AssetList assetList, Asset **selectedAsset);

void handleRead(AssetList assetList);
void handleAssetSelection(AssetList assetList, Asset **selectedAsset);
void handleSingleExrateSearch(AssetList assetList, Asset *selectedAsset);
void handleMultiExrateSearch(AssetList assetList, Asset *selectedAsset, int withInterval);
void handleBSTbalance(Asset *selectedAsset);

int main(int argc, char const *argv[])
{
    Command command;
    AssetList assetList =  AssetList_init();
    Asset *selectedAsset = NULL;

    do {
        if (selectedAsset != NULL){
            printf("\n");
            Asset_printShort(stdout, *selectedAsset, 0);
        }
        command = readCommand();
        handleCommand(command, assetList, &selectedAsset);
    } while (command != end);

    AssetList_free(assetList);

    return 0;
}

Command readCommand(void) {
    int valid = 0;
    Command command;

    while(!valid) {
        showCommands();
        printf("--> ");
        scanf("%d", &command);
        if (command >= 0 && command <= 6)
            valid = 1;
        else
            printf("ERROR, command not recognized\n");
    }

    return command;
}

void showCommands() {
    printf("\nMENU'\n");
    printf("0 - Stop\n"); 
    printf("1 - Read file\n");
    printf("2 - Select asset\n");
    printf("3 - Find daily exrate\n");
    printf("4 - Min max exrate in interval\n");
    printf("5 - Min max exrate globally\n");
    printf("6 - Rebalance database\n");
}

void handleCommand(Command command, AssetList assetList, Asset **selectedAsset) {
    switch (command) {
        case readf:
            handleRead(assetList);
            break;
        case asset:
            handleAssetSelection(assetList, selectedAsset);
            break;
        case singleExrate:
            handleSingleExrateSearch(assetList, *selectedAsset);
            break;
        case minmaxExrateInterval:
            handleMultiExrateSearch(assetList, *selectedAsset, 1);
            break;
        case minmaxExrate:
            handleMultiExrateSearch(assetList, *selectedAsset, 0);
            break;
        case balance:
            handleBSTbalance(*selectedAsset);
            break;
        default:
            break;
    }
}

void handleRead(AssetList assetList) {
    FILE *fp;
    Asset tmpAsset;
    char filename[MAX_LEN+1];
    int N, i;

    printf("Filename: ");
    scanf("%s", filename);

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("ERROR opening input file\n");
        return;
    }

    fscanf(fp, "%d", &N);
    for (i = 0; i < N; i++) {
        tmpAsset = Asset_scan(fp);
        if (AssetList_insert(assetList, tmpAsset)) {
            Asset_free(tmpAsset);
        }
    }

    fclose(fp);
}

void handleAssetSelection(AssetList assetList, Asset **selectedAsset) {
    int idx;
    
    if (AssetList_isEmpty(assetList))
        printf("No assets in memory, please load one from a file first\n");
    else {
        printf("\nASSET SELECTION\n");
        AssetList_show(stdout, assetList);
        printf("Select an asset: ");
        scanf("%d", &idx);

        if ((*selectedAsset = AssetList_getByIndex(assetList, idx)) == NULL) {
            printf("Asset not found!\n");
        }
    }

}

void handleSingleExrateSearch(AssetList assetList, Asset *selectedAsset) {
    if (selectedAsset == NULL)
        printf("No asset selected, please select one\n");
    else {

    }
}

void handleMultiExrateSearch(AssetList assetList, Asset *selectedAsset, int withInterval) {
    if (selectedAsset == NULL)
        printf("No asset selected, please select one\n");
    else {
        
    }

}

void handleBSTbalance(Asset *selectedAsset) {
    if (selectedAsset == NULL)
        printf("No asset selected, please select one\n");
    else
        ExrateBST_balance(Asset_getExrates(*selectedAsset));
}
