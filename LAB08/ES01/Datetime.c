#include "Datetime.h"

static int Datetime_dateToInt(Datetime a) {
    return a.year * 10000 + a.month * 100 + a.day;
}

int compareDate(Datetime a, Datetime b) {
    return Datetime_dateToInt(a) - Datetime_dateToInt(b);
}

Datetime Datetime_null() {
    Datetime date;

    date.day = date.hour = date.min = date.month = date.year = 0;

    return date;
}

Datetime Datetime_read() {
    Datetime date = Datetime_null();
    printf("Write a day yyy/mm/dd: ");
    scanf("%d/%d/%d", &date.year, &date.month, &date.day);

    return date;
}