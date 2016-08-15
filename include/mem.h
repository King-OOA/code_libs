#pragma once

#include <stdint.h>
#define CHECK 0


/* 分配nbytes字节的内存空间,不初始化 */
extern void *mem_alloc(int64_t nbytes, const char *file, int32_t line);
#define MALLOC(nbytes) mem_alloc((nbytes), __FILE__, __LINE__)
#define NEW(p) ((p) = MALLOC((int64_t)sizeof *(p)))
#define VNEW(p, num, type) ((p) = ALLOC((int64_t) (sizeof *(p) + (num) * sizeof(type))))

/* 分配count个元素的数组,每个元素大小为nbytes,数组初始化为0 */
extern void *mem_calloc(int64_t count, int64_t nbytes, const char *file, int32_t line);
#define CALLOC(count, nbytes) mem_calloc((count), (nbytes), __FILE__, __LINE__)
#define VNEW0(p, num, type) ((p) = CALLOC(1, (int64_t)(sizeof *(p) + (num) * sizeof(type))))

/* 释放p所指向的内存,将p置为NULL */
extern void mem_free(void *p,const char *file, int32_t line);
#define FREE(p) ((void) mem_free((p), __FILE__, __LINE__), (p) = NULL) /* p会被多次求值 */

extern void *mem_resize(void *p, int64_t nbytes, const char *file, int32_t line);
#define RESIZE(p, nbytes) ((p) = mem_resize((p), __FILE__, __LINE__)) /* p会被多次求值 */
