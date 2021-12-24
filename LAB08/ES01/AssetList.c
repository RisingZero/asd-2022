#include "AssetList.h"

typedef struct node_ *link;
struct node_ {
    Asset asset;
    link next;
};

struct asset_list_ {
    link headP;
    int N;
};

AssetList AssetList_init() {
    AssetList list = (AssetList) malloc(sizeof (*list));
    list->headP = NULL;
    list->N = 0;

    return list;
}

static void freeR(link h) {
    if (h == NULL) return;

    freeR(h->next);
    Asset_free(h->asset);
    free(h);
}

void AssetList_free(AssetList assetList) {
    freeR(assetList->headP);
    free(assetList);
}

static link NEW(Asset asset, link next) {
    link p = (link) malloc(sizeof(*p));
    p->asset = asset;
    p->next = next;

    return p;
}

static void showR(FILE *fp, link p) {
    Asset_printShort(fp, p->asset);
    showR(fp, p->next);
}

void AssetList_show(FILE *fp, AssetList assetList) {
    if (assetList->N == 0) {
        printf("No assets\n");
    } else {
        showR(fp, assetList->headP);
    }
}

int AssetList_isEmpty(AssetList assetList) {
    return assetList->N == 0;
}

void AssetList_insert(AssetList assetList, Asset asset) {
    link *xp, t;

    for (*xp = assetList->headP; *xp != NULL; xp = &((*xp)->next)) {
        if (Asset_KeyCompare(Asset_getKey((*xp)->asset), Asset_getKey(asset)) > 0) {
            break;
        }
    }
    t = NEW(asset, (*xp)->next);
    *xp = t;

    assetList->N++;
}

Asset *AssetList_getByIndex(AssetList assetList, int index) {
    link h;
    int i = 0;
    
    h = assetList->headP;
    while (h != NULL) {
        if (i == index)
            return &(h->asset);
        h = h->next;
        i++;
    }
    return NULL;
}
