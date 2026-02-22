#include <assert.h>
#include <string.h>
#include <stdio.h>

#define STR_IMPLEMENTATION
#include "str.h"


int main(void)
{
    str empty = { .data = NULL, .length = 0 };
    str hello = cstr("hello");
    str world = cstr("world");
    str hell  = cstr("hell");
    str ello  = cstr("ello");
    str lo    = cstr("lo");
    str o     = cstr("o");
    str llo   = cstr("llo");
    str hello2 = cstr("hello");

    /* str_match */
    assert(str_match(hello, hello2));
    assert(!str_match(hello, world));
    assert(!str_match(hello, hell));
    assert(!str_match(empty, hello));
    assert(!str_match(hello, empty));
    assert(!str_match(empty, empty));

    /* str_find_first */
    assert(str_find_first(hello, hell) == 0);
    assert(str_find_first(hello, ello) == 1);
    assert(str_find_first(hello, lo)   == 3);
    assert(str_find_first(hello, o)    == 4);

    assert(str_find_first(hello, world) == -1);
    assert(str_find_first(hello, empty) == -1);
    assert(str_find_first(empty, hello) == -1);
    assert(str_find_first(empty, empty) == -1);

    /* str_find_last */
    str banana = cstr("banana");
    str na = cstr("na");

    assert(str_find_last(banana, na) == 4);
    assert(str_find_last(banana, cstr("ba")) == 0);
    assert(str_find_last(banana, cstr("a")) == 5);

    assert(str_find_last(banana, world) == -1);
    assert(str_find_last(banana, empty) == -1);
    assert(str_find_last(empty, na) == -1);

    /* str_contains */
    assert(str_contains(hello, hell));
    assert(str_contains(hello, lo));
    assert(!str_contains(hello, world));
    assert(!str_contains(hello, empty));
    assert(!str_contains(empty, hello));

    /* str_sub */
    str sub1 = str_sub(hello, 0, 5);
    assert(sub1.length == 5);
    assert(memcmp(sub1.data, "hello", 5) == 0);

    str sub2 = str_sub(hello, 1, 4);
    assert(sub2.length == 3);
    assert(memcmp(sub2.data, "ell", 3) == 0);

    str sub3 = str_sub(hello, 5, 5);
    assert(sub3.length == 0);

    str sub4 = str_sub(hello, 4, 10);
    assert(sub4.data == NULL && sub4.length == 0);

    str sub5 = str_sub(empty, 0, 0);
    assert(sub5.data == NULL && sub5.length == 0);

    /* str_starts_with */
    assert(str_starts_with(hello, hell));
    assert(str_starts_with(hello, hello));
    assert(!str_starts_with(hello, ello));
    assert(!str_starts_with(hello, world));
    assert(!str_starts_with(empty, hello));

    /* str_ends_with */
    assert(str_ends_with(hello, lo));
    assert(str_ends_with(hello, o));
    assert(str_ends_with(hello, hello));
    assert(!str_ends_with(hello, hell));
    assert(!str_ends_with(empty, o));

    /* str_remove_prefix */
    str rp1 = str_remove_prefix(hello, hell);
    assert(rp1.length == 1);
    assert(memcmp(rp1.data, "o", 1) == 0);

    str rp2 = str_remove_prefix(hello, world);
    assert(rp2.data == hello.data && rp2.length == hello.length);

    str rp3 = str_remove_prefix(empty, hell);
    assert(rp3.data == empty.data && rp3.length == empty.length);

    /* str_remove_suffix */
    str rs1 = str_remove_suffix(hello, lo);
    assert(rs1.length == 3);
    assert(memcmp(rs1.data, "hel", 3) == 0);

    str rs2 = str_remove_suffix(hello, world);
    assert(rs2.data == hello.data && rs2.length == hello.length);

    str rs3 = str_remove_suffix(empty, lo);
    assert(rs3.data == empty.data && rs3.length == empty.length);


    printf("All str tests passed successfully.\n");
    return 0;
}
