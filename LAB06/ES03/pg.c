#include "pg.h"

int pg_read(FILE *fp, pg_t *pgp) {
    if (fscanf(fp, "%s %s %s", pgp->cod, pgp->nome, pgp->classe) == 3) {
        pgp->equip = equipArray_init();
        stat_read(fp, &(pgp->b_stat));
        stat_zeroed(&(pgp->eq_stat));
        return 1;
    } else
        return 0;

}

void pg_clean(pg_t *pgp) {
    equipArray_free(pgp->equip);
}

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray) {
    stat_t res_stat = stat_combine(pgp->b_stat, pgp->eq_stat);

    fprintf(fp, "%-6s | %-50s | %-50s | ", pgp->cod, pgp->nome, pgp->classe);
    stat_print(fp, &res_stat, 1);
    fprintf(fp, "\n");
}

void pg_updateEquip(pg_t *pgp, invArray_t invArray) {
    int i, idxObj;
    stat_t temp_stat;
    inv_t *temp_inv;

    stat_zeroed(&temp_stat);

    equipArray_print(stdout, pgp->equip, invArray);
    equipArray_update(pgp->equip, invArray);
    for (i = 0; i < equipArray_inUse(pgp->equip); i++) {
        idxObj = equipArray_getEquipByIndex(pgp->equip, i);
        temp_inv = invArray_getByIndex(invArray, idxObj);
        temp_stat = stat_combine(temp_stat, inv_getStat(temp_inv));
    }
    pgp->eq_stat = temp_stat;
}

void pg_header(FILE *fp) {
    fprintf(fp, "%6s | %-50s | %-50s | ",
        "CODICE", "NOME", "CLASSE"
    );
    stat_header(fp);
    fprintf(fp, "\n");
}

int pg_compare_code(pg_t pg, char *cod) {
  return strcmp(pg.cod, cod);
}
