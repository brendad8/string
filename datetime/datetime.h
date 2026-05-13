


#ifndef DATETIME_H
#define DATETIME_H

#include <time.h>

typedef struct
{
    time_t _time; // _time, _cal not meant to be accessed directly
    struct tm _cal;

} Datetime;

bool datetime_parse_mm_dd_yy(const char *str, Datetime *d);
Datetime datetime_create(int year, int month, int mday, int hour, int min, int sec);
bool is_valid_datetime(int year, int month, int day, int hour, int min, int sec);

Datetime datetime_now(void);

int datetime_get_year(Datetime d);
int datetime_get_month(Datetime d);
int datetime_get_mday(Datetime d);
int datetime_get_wday(Datetime d);
int datetime_get_hour(Datetime d);
int datetime_get_min(Datetime d);

Datetime datetime_add_secs(Datetime d, int num_secs);
Datetime datetime_sub_secs(Datetime d, int num_secs);
Datetime datetime_add_mins(Datetime d, int num_mins);
Datetime datetime_sub_mins(Datetime d, int num_mins);
Datetime datetime_add_hours(Datetime d, int num_hours);
Datetime datetime_sub_hours(Datetime d, int num_hours);
Datetime datetime_add_days(Datetime d, int num_days);
Datetime datetime_sub_days(Datetime d, int num_days);

int   datetime_diff_secs(Datetime d1, Datetime d2);
float datetime_diff_mins(Datetime d1, Datetime d2);
float datetime_diff_hours(Datetime d1, Datetime d2);
float datetime_diff_days(Datetime d1, Datetime d2);


#endif // DATETIME_H
