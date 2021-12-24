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
    AssetList list = (AssetList) malloc(sizeof(*list));
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

static void showR(FILE *fp, link p, int *cnt) {
    if (p == NULL)
        return;
    
    Asset_printShort(fp, p->asset, *cnt);
    (*cnt)++;
    showR(fp, p->next, cnt);
}

void AssetList_show(FILE *fp, AssetList assetList) {
    int cnt = 0;

    if (assetList->N == 0) {
        printf("No assets\n");
    } else {
        showR(fp, assetList->headP, &cnt);
    }
}

int AssetList_isEmpty(AssetList assetList) {
    return assetList->N == 0;
}

int AssetList_insert(AssetList assetList, Asset asset) {
    link p, t;

    if (assetList->headP == NULL || Asset_KeyCompare(Asset_getKey(asset), Asset_getKey(assetList->headP->asset)) < 0) {
        assetList->headP = NEW(asset, assetList->headP);
        assetList->N++;
        return 0;
    }

    if (Asset_KeyCompare(Asset_getKey(assetList->headP->asset), Asset_getKey(asset)) == 0) {
        ExrateBST_merge(Asset_getExrates(assetList->headP->asset), Asset_getExrates(asset));
        return 1;
    }

    for (p = assetList->headP, t = p->next; t != NULL; p = t,  t = t->next) {
        if (Asset_KeyCompare(Asset_getKey(t->asset), Asset_getKey(asset)) == 0) {
            ExrateBST_merge(Asset_getExrates(t->asset), Asset_getExrates(asset));
            return 1;
        }
        if (Asset_KeyCompare(Asset_getKey(t->asset), Asset_getKey(asset)) > 0) {
            break;
        }
    }
    p->next = NEW(asset, t);
    assetList->N++;
    return 0;
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
