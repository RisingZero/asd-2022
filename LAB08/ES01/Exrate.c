#include "Exrate.h"

Exrate Exrate_read(FILE *fp) {
    Exrate t;

    fscanf(fp, "%d/%d/%d %d:%d %d %d", 
        &t.date.year, &t.date.month, &t.date.day, &t.date.hour, &t.date.min,
        &t.q, &t.n
    );

    return t;
}

Exrate Exrate_null() {
    Exrate t;

    t.q = 0;
    t.n = 0;
    t.date = Datetime_null();

    return t;
}

int Exrate_is_null(Exrate t) {
    return t.n == 0;
}

void Exrate_display(FILE *fp, Exrate t) {
    if (Exrate_is_null(t))
        return;

    fprintf(fp, "Day %d/%d/%d - Exchange rate: %d\n", t.date.year, t.date.month, t.date.day, t.q);
}