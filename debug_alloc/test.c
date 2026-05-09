

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USE_DEBUG_ALLOCATOR
#include "debug_alloc.h"


int main(void) {
    printf("=== allocation test start ===\n");

    // -----------------------------
    // 1. simple malloc/free
    // -----------------------------
    int* a = malloc(5 * sizeof(int));
    if (!a) return 1;

    for (int i = 0; i < 5; i++) {
        a[i] = i * 10;
    }

    printf("a[3] = %d\n", a[3]);

    free(a);
    a = NULL;

    // -----------------------------
    // 2. calloc test
    // -----------------------------
    int* b = calloc(10, sizeof(int));
    if (!b) return 1;

    printf("calloc b[7] = %d (should be 0)\n", b[7]);

    free(b);

    // -----------------------------
    // 3. realloc growth
    // -----------------------------
    int* c = malloc(4 * sizeof(int));
    if (!c) return 1;

    for (int i = 0; i < 4; i++) {
        c[i] = i + 1;
    }

    c = realloc(c, 10 * sizeof(int));
    if (!c) return 1;

    for (int i = 4; i < 10; i++) {
        c[i] = i + 1;
    }

    printf("c[9] = %d\n", c[9]);

    free(c);

    // -----------------------------
    // 4. realloc shrink
    // -----------------------------
    int* d = malloc(20 * sizeof(int));
    if (!d) return 1;

    for (int i = 0; i < 20; i++) {
        d[i] = i;
    }

    d = realloc(d, 5 * sizeof(int));
    if (!d) return 1;

    printf("d[4] = %d\n", d[4]);

    free(d);

    // -----------------------------
    // 5. intentional leak (for debugger)
    // -----------------------------
    int* leak = malloc(100 * sizeof(int));
    leak[0] = 42;

    printf("intentional leak allocated at %p\n", (void*)leak);

    // -----------------------------
    // 6. double free test (UNCOMMENT to test)
    // -----------------------------
    int* bad = malloc(10 * sizeof(int));
    free(bad);
    free(bad); // should be caught by debug allocator

    printf("=== allocation test end ===\n");

    debug_alloc_report();

    return 0;
}
