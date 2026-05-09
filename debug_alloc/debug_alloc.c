
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    void*  ptr;
    size_t size;
    size_t file_line;
    char*  file_name;
} Allocation;

typedef struct {
    Allocation* allocs;
    size_t len;
    size_t cap;
} AllocationList;

AllocationList alloc_list = {0};

static bool alloc_list_init(AllocationList* alloc_list);
static bool alloc_list_free(AllocationList* alloc_list);
static bool alloc_list_add(AllocationList* alloc_list, Allocation alloc);
static bool alloc_list_remove(AllocationList* alloc_list, void* ptr);
static int  alloc_list_get_alloc(AllocationList* alloc_list, void* ptr);

void* debug_malloc(size_t bytes, char* file, size_t line) {
    void* ptr = malloc(bytes);
    bool success = alloc_list_add(
        &alloc_list, 
        (Allocation){ .ptr = ptr, .size = bytes, .file_line = line, .file_name = file }
    );
    if (!success) {
        fprintf(stderr, "debug_malloc failed to add allocation to alloc_list:");
        fprintf(stderr, "  file = %s, line = %zu", file, line);
    }
    return ptr;
}

void* debug_realloc(void* ptr, size_t bytes, char* file, size_t line) {
    bool success = true;

    void* new_ptr = realloc(ptr, bytes);
    Allocation new_alloc = {
        .ptr = new_ptr,
        .size = bytes,
        .file_name = file,
        .file_line = line
    };

    if (new_ptr == NULL) 
        return new_ptr;

    int idx = alloc_list_get_alloc(&alloc_list, ptr);
    if (idx < 0)
        success = alloc_list_add(&alloc_list, new_alloc);
    else
        alloc_list.allocs[idx] = new_alloc;

    if (!success) {
        fprintf(stderr, "debug_realloc failed to add allocation to alloc_list:\n");
        fprintf(stderr, "  file = %s, line = %zu\n", file, line);
    }
    return new_ptr;

}

void* debug_calloc(size_t n_elt, size_t size_elt, char* file, size_t line) {
    void* ptr = calloc(n_elt, size_elt);
    bool success = alloc_list_add(
        &alloc_list, 
        (Allocation){
            .ptr = ptr, 
            .size = n_elt * size_elt, 
            .file_line = line, 
            .file_name = file
        }
    );
    if (!success) {
        fprintf(stderr, "debug_calloc failed to add allocation to alloc_list:\n");
        fprintf(stderr, "  file = %s, line = %zu\n", file, line);
    }
    return ptr;
}

void debug_free(void* ptr, char* file, size_t line) {
    int idx = alloc_list_get_alloc(&alloc_list, ptr);
    if (idx < 0) {
        printf("DOUBLE FREE: %p size=%d file=%s line=%zu\n",
            ptr, -1, file, line
        );
        return;
    }
    bool success = alloc_list_remove(&alloc_list, ptr);
    if (!success) {
        fprintf(stderr, "debug_free failed to remove ptr from allocation list:\n");
        fprintf(stderr, "  file = %s, line = %zu\n", file, line);
    }
    free(ptr);
}
void debug_alloc_report(void) {
    for (size_t i = 0; i < alloc_list.len; i++) {
        printf("LEAK: %p size=%zu file=%s line=%zu\n",
            alloc_list.allocs[i].ptr,
            alloc_list.allocs[i].size,
            alloc_list.allocs[i].file_name,
            alloc_list.allocs[i].file_line);
    }
}

static bool alloc_list_init(AllocationList* alloc_list) {
    Allocation* ptr = (Allocation*)malloc(10 * sizeof(Allocation));
    if (ptr == NULL)
        return false;

    alloc_list->allocs = ptr;
    alloc_list->len = 0;
    alloc_list->cap = 10;
    return true;
}

static bool alloc_list_free(AllocationList* alloc_list) {
    if (alloc_list->allocs == NULL)
        return false;

    free(alloc_list->allocs);
    alloc_list->len = 0;
    alloc_list->cap = 0;
    return true;
}

static bool alloc_list_add(AllocationList* alloc_list, Allocation alloc) {
    // initialize allocation list on first add
    if (alloc_list->allocs == NULL) {
        if (!alloc_list_init(alloc_list))
            return false;
    }
    // double capacity when len hits cap
    if (alloc_list->len == alloc_list->cap) {
       size_t new_cap = alloc_list->cap * 2;
       Allocation* new_ptr = (Allocation*)realloc(
            alloc_list->allocs, 
            new_cap * sizeof(Allocation)
        );
       if (new_ptr == NULL)
           return false;
       alloc_list->allocs = new_ptr;
       alloc_list->cap = new_cap;
    }
    // add new allocation
    (alloc_list->allocs)[alloc_list->len] = alloc;
    alloc_list->len++;
    return true;
}

static bool alloc_list_remove(AllocationList* alloc_list, void* ptr) {
    int idx = -1;
    for (int i = 0; i < alloc_list->len; i++)
        if (ptr == (alloc_list->allocs)[i].ptr) {
            idx = i;
            break;
        }

    if (idx < 0) 
        return false;
    if (idx < alloc_list->len - 1) {
        // shift allocation elements down to fill in space of removed
        memmove(
            (alloc_list->allocs) + idx,                      // dest 
            (alloc_list->allocs) + (idx + 1),                // src
            sizeof(Allocation) * (alloc_list->len - idx - 1) // size
        );
    }
    alloc_list->len--;
    return true;
}

static int alloc_list_get_alloc( AllocationList* alloc_list, void* ptr) {
    for (int i = 0; i < alloc_list->len; i++)
        if (ptr == (alloc_list->allocs)[i].ptr) 
            return i;
    return -1;
}

