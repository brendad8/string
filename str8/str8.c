
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "str8.h"

//*****************************
// private function prototypes 
//*****************************
static size_t _cstr8_len(u8* c);
static int    _find_first_impl(u8* s_ptr, size_t s_len, u8* p_ptr, size_t p_len);
static int    _find_last_impl(u8* s_ptr, size_t s_len, u8* p_ptr, size_t p_len); 
static bool   _starts_with_impl(u8* s_ptr, size_t s_len, u8* p_ptr, size_t p_len);
static bool   _ends_with_impl(u8* s_ptr, size_t s_len, u8* p_ptr, size_t p_len);
static str8   _remove_prefix_impl(u8* s_ptr, size_t s_len, u8* p_ptr, size_t p_len);
static str8   _remove_suffix_impl(u8* s_ptr, size_t s_len, u8* p_ptr, size_t p_len); 

str8 str8_from_cstr(u8* ptr) 
{
    size_t len = _cstr8_len(ptr);
    return (str8){ .ptr = ptr, .len = len };
}

void str8_print(str8 s)
{
    printf("%.*s", str8_fmt(s));
}

void str8_println(str8 s)
{
    printf("%.*s\n", str8_fmt(s));
}

bool str8_eq(str8 a, str8 b) 
{
    if (a.len != b.len) 
        return false;

    return memcmp(a.ptr, b.ptr, a.len) == 0;
}

int str8_cmp(str8 a, str8 b) 
{
    size_t min_len = a.len < b.len ? a.len : b.len;
    int cmp = memcmp(a.ptr, b.ptr, min_len);

    return cmp != 0 ? cmp : (int)(a.len - b.len);
}


int str8_find_first(str8 s, str8 pattern)
{
    return _find_first_impl(s.ptr, s.len, pattern.ptr, pattern.len);
}


int str8_find_last(str8 s, str8 pattern)
{
    return _find_last_impl(s.ptr, s.len, pattern.ptr, pattern.len);
}

bool str8_contains(str8 s, str8 pattern) 
{
    return str8_find_first(s, pattern) != -1;
}

str8 str8_sub(str8 s, size_t begin, size_t length) 
{
    if (begin > s.len - 1 || begin + length > s.len - 1)
        return (str8){ .ptr = NULL, .len = 0 };

    return (str8){.ptr = s.ptr + begin, .len = length};
}

bool str8_starts_with(str8 s, str8 prefix) 
{
    return _starts_with_impl(s.ptr, s.len, prefix.ptr, prefix.len);
}

bool str8_ends_with(str8 s, str8 suffix) 
{
    return _ends_with_impl(s.ptr, s.len, suffix.ptr, suffix.len);
}

str8 str8_remove_prefix(str8 s, str8 prefix)
{
    return _remove_prefix_impl(s.ptr, s.len, prefix.ptr, prefix.len);
}

str8 str8_remove_suffix(str8 s, str8 prefix)
{
    return _remove_suffix_impl(s.ptr, s.len, prefix.ptr, prefix.len);
}

str8 str8_trim_left(str8 s)
{
    size_t trim_off = 0;
    while (isspace(*(s.ptr + trim_off)))
        trim_off++;

    return (str8){ .ptr = s.ptr + trim_off, .len = s.len - trim_off }; 
}

str8 str8_trim_right(str8 s)
{
    size_t trim_off = 0;
    while (isspace(*(s.ptr + s.len - trim_off)))
        trim_off++;

    return (str8){ .ptr = s.ptr, .len = s.len - trim_off }; 
}

str8 str8_trim(str8 s)
{
    str8 tmp = str8_trim_left(s);
    return str8_trim_right(tmp);
}

str8_iter str_split_iter(str8 s, str8 delim)
{
    return (str8_iter){
        .iter_str = s, 
        .delim = delim,
        .sub = (str8){ .ptr = NULL, .len = 0 }
    };
}

bool str8_iter_next(str8_iter* iter)
{
    str8 iter_str = (iter->iter_str);

    if (iter_str.ptr == NULL)
        return false;

    int idx = str8_find_first(iter_str, iter->delim);

    if (idx == -1) // delim not found
    {
        iter->sub = (str8){ .ptr = iter_str.ptr, .len = iter_str.len };
        iter->iter_str = (str8){.ptr = NULL, .len = 0 };
    } 
    else if (idx == 0) // empty split
    {
        iter->sub = (str8){ .ptr = NULL, .len = 0 };
        (iter->iter_str).ptr += idx + (iter->delim).len;
        (iter->iter_str).len -= idx + (iter->delim).len;
    } 
    else // delim found
    {
        iter->sub = (str8){ .ptr = iter_str.ptr, .len = (size_t)idx };
        (iter->iter_str).ptr += idx + (iter->delim).len;
        (iter->iter_str).len -= idx + (iter->delim).len;
    }
    return true;
}

// strbuf strbuf_from_ptr(char* ptr) 
// {
//     if (ptr == NULL)
//         return (strbuf){ .ptr = NULL, .len = 0, .cap = 0 };
//
//     size_t len = strlen(ptr);
//     size_t cap = (3 * (len + 1)) / 2; // set cap as 1.5x size of str
//
//     char* new_ptr = malloc(cap);
//     memcpy(new_ptr, ptr, len + 1);
//
//     return (strbuf){ .ptr = new_ptr, .len = len, .cap = cap };
// }
//
// strbuf strbuf_from_str(str s)
// {
//     if (s.ptr == NULL)
//         return (strbuf){ .ptr = NULL, .len = 0, .cap = 0 };
//
//     size_t len = strlen(s.ptr);
//     size_t cap = (3 * (len + 1)) / 2; // set cap as 1.5x size of str
//
//     char* new_ptr = malloc(cap);
//     memcpy(new_ptr, s.ptr, len + 1);
//
//     return (strbuf){ .ptr = new_ptr, .len = len, .cap = cap };
// }
//
// void strbuf_free(strbuf* s)
// {
//     if (s->ptr == NULL) 
//         return;
//
//     free(s->ptr);
//     s->len = 0; 
//     s->cap = 0;
// }
//
// int strbuf_cmp(strbuf a, strbuf b) 
// {
//     size_t min_len = a.len < b.len ? a.len : b.len;
//     int cmp = strncmp(a.ptr, b.ptr, min_len);
//
//     return cmp != 0 ? cmp : (int)(a.len - b.len);
// }
//
// bool strbuf_eq(strbuf a, strbuf b) 
// {
//     if (a.len != b.len) 
//         return false;
//
//     return memcmp(a.ptr, b.ptr, a.len) == 0;
// }
//
// int strbuf_find_first(strbuf s, str pattern)
// {
//     return find_first_impl(s.ptr, s.len, pattern.ptr, pattern.len);
// }
//
// int strbuf_find_last(strbuf s, str pattern)
// {
//     return find_last_impl(s.ptr, s.len, pattern.ptr, pattern.len);
// }
//
// bool strbuf_contains(strbuf s, str pattern) 
// {
//     return strbuf_find_first(s, pattern) != -1;
// }

// str strbuf_sub(strbuf* s, size_t begin, size_t length) 
// {
//     if (begin > s.len - 1 || begin + length > s.len - 1)
//         return (str){ .ptr = NULL, .len = 0 };
//
//     return (str){.ptr = s.ptr + begin, .len = length};
// }
//
// str strbuf_sub_to_str(strbuf* s, size_t begin, size_t length) 
// {
//     if (begin > s.len - 1 || begin + length > s.len - 1)
//         return (str){ .ptr = NULL, .len = 0 };
//
//     return (str){.ptr = s.ptr + begin, .len = length};
// }
//
// bool strbuf_starts_with(strbuf s, str prefix) 
// {
//     return _starts_with_impl(s.ptr, s.len, prefix.ptr, prefix.len);
// }
//
// bool strbuf_ends_with(strbuf s, str suffix) 
// {
//     return _ends_with_impl(s.ptr, s.len, suffix.ptr, suffix.len);
// }
//
// str8 strbuf_remove_prefix(strbuf s, str prefix)
// {
//     return _remove_prefix_impl(s.ptr, s.len, prefix.ptr, prefix.len);
// }
//
// str8 strbuf_remove_suffix(strbuf s, str prefix)
// {
//     return _remove_suffix_impl(s.ptr, s.len, prefix.ptr, prefix.len);
// }


//****************************
// private functions
//****************************

static size_t _cstr8_len(u8* c)
{
    size_t len = 0;
    if(c)
    {
        u8 *p = c;
        for (; *p != 0; p += 1);
        len = (size_t)(p - c);
    }
    return len;
}

static int _find_first_impl(u8* s_ptr, size_t s_len, u8* p_ptr, size_t p_len) 
{
    if (s_len < p_len || p_len == 0) 
        return -1;

    for (int i = 0; i <= (s_len - p_len); i++) 
    {
        for (int j = 0; j < p_len; j++) 
        {
            if (s_ptr[i+j] != p_ptr[j]) 
                break; 
            if (j == p_len-1)
                return i;
        }
    }
    return -1;
}

static int _find_last_impl(u8* s_ptr, size_t s_len, u8* p_ptr, size_t p_len) 
{
    if (s_len < p_len || p_len == 0) 
        return -1;

    for (int i = s_len - 1; i >= p_len - 1; i--) 
    {
        for (int j = 0; j < p_len; j++) 
        {
            if (s_ptr[i-j] != p_ptr[p_len - j - 1]) 
                break; 
            if (j == p_len - 1)
                return (i - p_len + 1);
        }
    }
    return -1;
}

static bool _starts_with_impl(u8* s_ptr, size_t s_len, u8* p_ptr, size_t p_len)
{
    if (s_len < p_len)
        return false;

    return memcmp(s_ptr, p_ptr, p_len) == 0;
}

static bool _ends_with_impl(u8* s_ptr, size_t s_len, u8* p_ptr, size_t p_len)
{
    if (s_len < p_len)
        return false;

    return memcmp(s_ptr + (s_len - p_len), p_ptr, p_len) == 0;
}

static str8 _remove_prefix_impl(u8* s_ptr, size_t s_len, u8* p_ptr, size_t p_len) 
{
    if (s_len < p_len)
        return (str8){ .ptr = s_ptr, .len = s_len };

    if (memcmp(s_ptr, p_ptr, p_len) != 0)
        return (str8){ .ptr = s_ptr, .len = s_len };
    
    return (str8){ .ptr = s_ptr + p_len, .len = s_len - p_len };
}

static str8 _remove_suffix_impl(u8* s_ptr, size_t s_len, u8* p_ptr, size_t p_len) 
{
    if (s_len < p_len)
        return (str8){ .ptr = s_ptr, .len = s_len };

    if (memcmp(s_ptr + s_len - p_len, p_ptr, p_len) != 0)
        return (str8){ .ptr = s_ptr, .len = s_len };
    
    return (str8){ .ptr = s_ptr, .len = s_len - p_len };
}
