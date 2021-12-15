#include "inv.h"

void stat_read(FILE *fp, stat_t *statp) {
    fscanf(fp, "%d %d %d %d %d %d",
            &(statp->hp),
            &(statp->mp),
            &(statp->atk),
            &(statp->def),
            &(statp->mag),
            &(statp->spr)
    );
}

void stat_print(FILE *fp, stat_t *statp, int soglia) {
    fprintf(fp, "%4d %4d %4d %4d %4d %4d",
        (soglia && statp->hp < MIN_STAT) ? MIN_STAT : statp->hp,
        (soglia && statp->mp < MIN_STAT) ? MIN_STAT : statp->mp,
        (soglia && statp->atk < MIN_STAT) ? MIN_STAT : statp->atk,
        (soglia && statp->def < MIN_STAT) ? MIN_STAT : statp->def,
        (soglia && statp->mag < MIN_STAT) ? MIN_STAT : statp->mag,
        (soglia && statp->spr < MIN_STAT) ? MIN_STAT : statp->spr 
    );
}

void stat_zeroed(stat_t *statp) {
    statp->hp = statp->def = statp->mag = statp->atk = statp->mp = statp->spr = 0;
}

stat_t stat_combine(stat_t stat1, stat_t stat2) {
    stat_t res;

    res.hp = stat1.hp + stat2.hp;
    res.atk = stat1.atk + stat2.atk;
    res.def = stat1.def + stat2.def;
    res.mag = stat1.mag + stat2.mag;
    res.mp = stat1.mp + stat2.mp;
    res.spr = stat1.spr + stat2.spr;

    return res;
}

void inv_read(FILE *fp, inv_t *invp) {
    fscanf(fp, "%s %s", invp->nome, invp->tipo);
    stat_read(fp, &(invp->stat));
}

void inv_print(FILE *fp, inv_t *invp) {
    fprintf(fp, "%-50s | %-50s | ", invp->nome, invp->tipo);
    stat_print(fp, &(invp->stat), 0);
    fprintf(fp, "\n");
}

stat_t inv_getStat(inv_t *invp) {
    return invp->stat;
}

void inv_header(FILE *fp) {
    fprintf(fp, "%-50s | %-50s | ",
        "NOME", "TIPO"
    );
    stat_header(fp);
    fprintf(fp, "\n");
}

void stat_header(FILE *fp) {
    fprintf(fp, "%4s %4s %4s %4s %4s %4s",
        "HP", "MP", "ATK", "DEF", "MAG", "SPR"
    );
}

int inv_nameCompare(inv_t *invp1, char *name) {
  return (strcmp(invp1->nome, name) == 0);
}