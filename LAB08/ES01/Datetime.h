/* 
    Datetime module
 */
#ifndef DATETIME_MODULE
#define DATETIME_MODULE

typedef struct _datetime {
    int year, month, day;
    int hour, min;
} Datetime;

int compareDate(Datetime a, Datetime b);

Datetime Datetime_null();

Datetime Datetime_read();

#endif