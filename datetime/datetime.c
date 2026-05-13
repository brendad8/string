
#include <stdbool.h>
#include <time.h>
#include <stdio.h>

#include "datetime.h"

enum {
    SECONDS_PER_MINUTE = 60,
    SECONDS_PER_HOUR   = 3600,
    SECONDS_PER_DAY    = 86400
};

static Datetime _dt_add_secs(Datetime d, int num_secs);
static Datetime _dt_sub_secs(Datetime d, int num_secs);
static bool _is_leap_year(int year);
static int  _days_in_month(int year, int month);
static bool _date_is_valid(int year, int month, int day, int hour, int min, int sec);

Datetime datetime_create(
    int year, 
    int month, 
    int mday, 
    int hour, 
    int min, 
    int sec
) {
    // assumes inputs make a valid datetime
    // and is in local time
    struct tm cal_local = {
        .tm_year = year  - 1900,
        .tm_mon  = month - 1,
        .tm_mday = mday,
        .tm_hour = hour,
        .tm_min  = min,
        .tm_sec  = sec
    };

    time_t time_utc   = mktime(&cal_local);
    struct tm cal_utc = *gmtime(&time_utc);

    return (Datetime){ ._time = time_utc, ._cal = cal_utc };
}

Datetime datetime_now(void)
{
    time_t t = time(NULL);
    struct tm cal = *gmtime(&t);
    return (Datetime){ ._time = t, ._cal = cal };
}

bool datetime_parse_mm_dd_yy(const char *str, Datetime *d) 
{
    if (!str || !d) return 0;

    int month, day, year;
    if (sscanf(str, "%d-%d-%d", &month, &day, &year) != 3)
        return false;

    year = year + 2000;  // assume date is in 2000s

    if (!is_valid_datetime(year, month, day, 0, 0, 0))
        return false;

    Datetime tmp = datetime_create(year, month, day, 0, 0, 0); 
    d->_time = tmp._time;
    d->_cal  = tmp._cal;

    return true;
}

int datetime_get_year(Datetime d)  { return d._cal.tm_year + 1900; }
int datetime_get_month(Datetime d) { return d._cal.tm_mon + 1; }
int datetime_get_mday(Datetime d)  { return d._cal.tm_mday; }
int datetime_get_wday(Datetime d)  { return d._cal.tm_wday; }
int datetime_get_hour(Datetime d)  { return d._cal.tm_hour; }
int datetime_get_min(Datetime d)   { return d._cal.tm_min; }

Datetime datetime_add_secs(Datetime d, int num_secs)   { return _dt_add_secs(d, num_secs); }
Datetime datetime_sub_secs(Datetime d, int num_secs)   { return _dt_sub_secs(d, num_secs); }
Datetime datetime_add_mins(Datetime d, int num_mins)   { return _dt_add_secs(d, num_mins  * SECONDS_PER_MINUTE); }
Datetime datetime_sub_mins(Datetime d, int num_mins)   { return _dt_sub_secs(d, num_mins  * SECONDS_PER_MINUTE); }
Datetime datetime_add_hours(Datetime d, int num_hours) { return _dt_add_secs(d, num_hours * SECONDS_PER_HOUR); }
Datetime datetime_sub_hours(Datetime d, int num_hours) { return _dt_sub_secs(d, num_hours * SECONDS_PER_HOUR); }
Datetime datetime_add_days(Datetime d, int num_days)   { return _dt_add_secs(d, num_days  * SECONDS_PER_DAY); }
Datetime datetime_sub_days(Datetime d, int num_days)   { return _dt_sub_secs(d, num_days  * SECONDS_PER_DAY); }

int   datetime_diff_secs(Datetime d1, Datetime d2)  { return difftime(d2._time, d1._time); }
float datetime_diff_mins(Datetime d1, Datetime d2)  { return (float)difftime(d2._time, d1._time) / (float)SECONDS_PER_MINUTE; }
float datetime_diff_hours(Datetime d1, Datetime d2) { return (float)difftime(d2._time, d1._time) / (float)SECONDS_PER_HOUR; }
float datetime_diff_days(Datetime d1, Datetime d2)  { return (float)difftime(d2._time, d1._time) / (float)SECONDS_PER_DAY; }


static Datetime _dt_add_secs(Datetime d, int num_secs)
{
    time_t new_time = d._time + num_secs;
    struct tm new_cal = *gmtime(&new_time);
    return (Datetime){ ._time = new_time, ._cal = new_cal };
}

static Datetime _dt_sub_secs(Datetime d, int num_secs)
{
    time_t new_time = d._time - num_secs;
    struct tm new_cal = *gmtime(&new_time);
    return (Datetime){ ._time = new_time, ._cal = new_cal };
}

bool is_valid_datetime(int year, int month, int day, 
                       int hour, int min, int sec) 
{
    if (year <= 0) return false;
    if (month < 1 || month > 12) return false;
    if (day   < 1 || day   > _days_in_month(year, month)) return false;
    if (hour  < 0 || hour  > 23) return false;
    if (min   < 0 || min   > 59) return false;

    return true;
}

static bool _is_leap_year(int year) 
{
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

static int _days_in_month(int year, int month) 
{
    static const int days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && _is_leap_year(year))
        return 29;
    return days[month - 1];
}


