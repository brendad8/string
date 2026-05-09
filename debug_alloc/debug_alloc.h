
#ifndef DEBUG_ALLOCATOR_H
#define DEBUG_ALLOCATOR_H

#include <stddef.h>

void* debug_malloc(size_t bytes, char* file, size_t line);
void* debug_realloc(void* ptr, size_t bytes, char* file, size_t line);
void* debug_calloc(size_t n_elt, size_t size_elt, char* file, size_t line);
void  debug_free(void* ptr, char* file, size_t line);
void  debug_alloc_report(void);

#ifdef USE_DEBUG_ALLOCATOR

#define malloc(bytes) debug_malloc(bytes, __FILE__, __LINE__)
#define realloc(ptr, bytes) debug_realloc(ptr, bytes, __FILE__, __LINE__)
#define calloc(n, size) debug_calloc(n, size, __FILE__, __LINE__)
#define free(ptr) debug_free(ptr, __FILE__, __LINE__)

#endif

#endif // DEBUG_ALLOCATOR_H
