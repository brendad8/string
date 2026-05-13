
#ifndef STR_H
#define STR_H

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t u8;

typedef struct 
{ 
    u8*    ptr; 
    size_t len; 

} str8;

typedef struct
{ 
    str8 sub; 
    str8 iter_str; 
    str8 delim; 

} str8_iter;


#define str8(lit)   ((str8){ .ptr = (u8*)(lit), .len = sizeof((lit)) - 1 })
#define str8_fmt(s) (int)(s).len, (s).ptr

str8      str8_from_cstr(u8* ptr);
void      str8_print(str8 s);
void      str8_println(str8 s);
bool      str8_eq(str8 a, str8 b);
int       str8_cmp(str8 a, str8 b);
bool      str8_contains(str8 s, str8 pattern);
bool      str8_starts_with(str8 s, str8 prefix);
bool      str8_ends_with(str8 s, str8 suffix);
int       str8_find_first(str8 s, str8 pattern);
int       str8_find_last(str8 s, str8 pattern);
str8      str8_sub(str8 s, size_t begin, size_t length);
str8      str8_remove_prefix(str8 s, str8 prefix);
str8      str8_remove_suffix(str8 s, str8 suffix);
str8      str8_trim(str8 s);
str8      str8_trim_left(str8 s);
str8      str8_trim_right(str8 s);
str8_iter str8_split_iter(str8 s, str8 delim);
bool      str8_iter_next(str8_iter* iter);


// strbuf strbuf_from_ptr(char* ptr);
// strbuf strbuf_from_str(str s);
// void   strbuf_free(strbuf* s);
// bool   strbuf_eq(strbuf a, strbuf b);
// int    strbuf_cmp(strbuf a, strbuf b);
// bool   strbuf_contains(strbuf s, str pattern);
// bool   strbuf_starts_with(strbuf s, str prefix);
// bool   strbuf_ends_with(strbuf s, str suffix);
// int    strbuf_find_first(strbuf s, str pattern);
// int    strbuf_find_last(strbuf s, str pattern);
// str    strbuf_sub(strbuf s, size_t begin, size_t length);
// str    strbuf_remove_prefix(strbuf s, str prefix);
// str    strbuf_remove_suffix(strbuf s, str suffix);
// void   strbuf_clear(strbuf* s);
// void   strbuf_reserve(strbuf* s, size_t new_cap);
// void   strbuf_resize(strbuf* s, size_t new_len);
// void   strbuf_append(strbuf* s, str other);
// void   strbuf_append_fmt(strbuf* s, const char* fmt, ...);
// void   strbuf_append_ptr(strbuf* s, const char* cstr);
// void   strbuf_append_char(strbuf* s, char c);
// void   strbuf_replace(strbuf* s, str target, str replacement);
// strbuf strbuf_copy(strbuf s);

#endif /* STR_H */
