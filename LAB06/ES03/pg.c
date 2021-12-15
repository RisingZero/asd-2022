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
    equipArray_update(pgp->equip, invArray);
    // TODO: implementation of eq_stat update after equipUpdate
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
