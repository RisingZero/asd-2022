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