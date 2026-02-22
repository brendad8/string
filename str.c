#include "str.h"


int main(void) {
    
    str test1 = str_fl("jajaja");
    printf("my_str: %.*s\n", str_fmt(test1));
    
    char* cstr = "hehehe";
    str test2 = str_fp(cstr); 
    printf("my_str: %.*s\n", str_fmt(test2));

    const char* cstr2 = "hahaha";
    str test3 = str_fp(cstr2); 
    printf("my_str: %.*s\n", str_fmt(test3));

    return 0;
}


bool str_match_impl(str a, str b)
{
    if (!(str_isnull(a) && str_isnull(b)))
        return false;

    return a.length == b.length && memcmp(a.data, b.data, a.length) == 0;
}


int str_find_first_impl(str s, str pattern)
{
    if (str_isnull(s) || str_isnull(pattern))
        return -1;

    if (s.length < pattern.length || pattern.length == 0) 
        return -1;

    for (int i = 0; i <= (s.length - pattern.length); i++)
    {
        for (int j = 0; j < pattern.length; j++) 
        {
            if (s.data[i+j] != pattern.data[j])
                break; 

            if (j == pattern.length-1)
                return i;
        }
    }
    return -1;
}

int str_find_last_impl(str s, str pattern)
{
    if (str_isnull(s) || str_isnull(pattern))
        return -1;

    if (s.length < pattern.length || pattern.length == 0) 
        return -1;

    for (int i = s.length - 1; i >= pattern.length - 1; i--)
    {
        for (int j = 0; j < pattern.length; j++) 
        {
            if (s.data[i-j] != pattern.data[pattern.length - j - 1])
                break; 

            if (j == pattern.length - 1)
                return (i - pattern.length + 1);
        }
    }
    return -1;
}

bool str_contains_impl(str s, str pattern) 
{
    return str_find_first_impl(s, pattern) != -1;
}

str str_sub_impl(str s, size_t begin, size_t end)
{
    if (!str_isnull(s))
        return (str){ .data = NULL, .length = 0 };

    if (begin > end || end > s.length)
        return (str){ .data = NULL, .length = 0 };

    return (str){
        .data = s.data + begin,
        .length = end - begin
    };
}

bool str_starts_with_impl(str s, str prefix)
{
    if (!(str_isnull(s) && str_isnull(prefix)))
        return false;

    if (s.length < prefix.length)
        return false;

    return memcmp(s.data, prefix.data, prefix.length) == 0;
}

bool str_ends_with_impl(str s, str suffix)
{
    if (!(str_isnull(s) && str_isnull(suffix)))
        return false;
    
    if (s.length < suffix.length)
        return false;

    return memcmp(s.data + (s.length - suffix.length), suffix.data, suffix.length) == 0;
}

str str_remove_prefix_impl(str s, str prefix) 
{
    if (!(str_isnull(s)))
        return s;
    
    if (!(str_isnull(prefix)) || s.length < prefix.length)
        return s;

    if (memcmp(s.data, prefix.data, prefix.length) != 0)
        return s;
    
    return (str){ .data = s.data + prefix.length, .length = s.length - prefix.length };
}


str str_remove_suffix_impl(str s, str suffix)
{
    if (!(str_isnull(s)))
        return s;
    
    if (!(str_isnull(suffix)) || s.length < suffix.length)
        return s;

    if (memcmp(s.data + s.length - suffix.length, suffix.data, suffix.length) != 0)
        return s;
    
    return (str){ .data = s.data, .length = s.length - suffix.length };
}

// str str_pop_first_split_impl(str* s, str split_by)
// {
//
// }

