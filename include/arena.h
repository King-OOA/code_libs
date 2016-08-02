#pragma once

#include <stdint.h>

#define T Pool_T
typedef struct T *T;

extern T pool_new(void); /* 构造新的内存池 */
extern void pool_dispose(T *ap); /* 销毁内存池 */

extern void *pool_alloc (T pool, int64_t nbytes, const char *file, int  line);
extern void *pool_calloc (T pool, int64_t count, int64_t bytes, const char *file, int  line);
extern void pool_free(T pool);

