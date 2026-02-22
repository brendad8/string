
#ifndef STR_H
#define STR_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct str 
{
    size_t length;
    const char* data;

} str;


#define str_fl(l) \
    ((str){ .data = (l), .length = sizeof((l)) - 1 })

#define str_fp(p) \
    ((str){ .data = (p), .length = strlen((p)) })

#define str_fmt(s) \
    (int)(s).length, (s).data

#define str_isnull(s) \
    ((s).data == NULL)

#define str_isempty(s) \
    (((s).data) ? (s).length == 0 : 1)

#define __str(x)                    \
    _Generic((x),                   \
        str:            (x),        \
        char*:          str_fp(x),  \
        const char*:    str_fp(x),  \
        default:        str_fl(x)   \
    )

#define str_find_first(s, p) \
    str_find_first_impl(strc(s), strc(p))

#define str_find_last(s, p) \
    str_find_first_impl(strc(s), strc(p))

#define str_match(a, b) \
    str_match_impl(strc(a), strc(b))

#define str_contains(s, p) \
    str_contains_impl(strc(s), strc(p))

#define str_starts_with(s, p) \
    str_starts_with_impl(strc(s), strc(p))

#define str_ends_with(s, p) \
    str_ends_with_impl(strc(s), strc(p))

#define str_sub(s, begin, end) \
    str_sub_impl(strc(s), (begin), (end))

#define str_remove_prefix(s, p) \
    str_remove_prefix_impl(strc(s), strc(p))

#define str_remove_suffix(s, p) \
    str_remove_suffix_impl(strc(s), strc(p))


int str_find_first_impl(str s, str pattern);
int str_find_last_impl(str s, str pattern);
bool str_match_impl(str a, str b);
bool str_contains_impl(str s, str pattern);
bool str_starts_with_impl(str s, str prefix);
bool str_ends_with_impl(str s, str suffix);
str  str_sub_impl(str s, size_t begin, size_t end);
str  str_remove_prefix_impl(str s, str prefix);
str  str_remove_suffix_impl(str s, str suffix);
str str_pop_first_split_impl(str* s, str split_by);

// int str_compare(str a, str b);
// str_from_ptr(char* data, size_t len);
// size_t str_index_of(str s, str pattern);


#endif /* STR_H */
