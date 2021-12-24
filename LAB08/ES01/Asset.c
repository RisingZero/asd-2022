#include "Asset.h"

struct asset_ {
    char code[CODE_LEN+1];
    ExrateBST dailyExrates;
};

Asset Asset_create() {
    Asset t = (Asset) malloc(sizeof(*t));
    t->dailyExrates = ExrateBST_init();
    return t;
}

void Asset_free(Asset t) {
    ExrateBST_free(t->dailyExrates);
    free(t);
}

Asset Asset_scan(FILE *fp) {
    int i, nAssets;
    Asset asset = Asset_create();
    Exrate exrate;

    fscanf(fp, "%s %d", asset->code, &nAssets);
    for (i = 0; i < nAssets; i++) {
        exrate = Exrate_read(fp);
        ExrateBST_insert(asset->dailyExrates, exrate);
    }

    return asset;
}

void Asset_printShort(FILE *fp, Asset asset) {
    fprintf(fp, "%s\n", asset->code);
}

AssetKey Asset_getKey(Asset t) {
    return t->code;
}

int Asset_KeyCompare(AssetKey k1, AssetKey k2) {
    return strcmp(k1, k2);
}

ExrateBST Asset_getExrates(Asset t) {
    return t->dailyExrates;
}