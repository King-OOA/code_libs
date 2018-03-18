#pragma once

#include <stdint.h>
#define CHECK 0


/* 分配count个type类型的内存空间,不初始化 */
extern void *mem_alloc(int64_t nbytes, const char *file, int32_t line);
#define MALLOC(count, type) (mem_alloc((count) * sizeof (type),\
				       __FILE__, __LINE__))
#define NEW(p) ((p) = mem_alloc(sizeof *(p),\
				__FILE__, __LINE__))
#define VNEW(p, count, type) ((p) = mem_alloc \
			    ((sizeof *(p) + (count) * sizeof(type)),\
			     __FILE__, __LINE__))

/* 分配count个元素的数组,每个元素大小为nbytes,数组初始化为0 */
extern void *mem_calloc(int64_t count, int64_t nbytes, const char *file, int32_t line);
#define CALLOC(count, type) (mem_calloc((count), sizeof (type),\
					__FILE__, __LINE__))
#define NEW0(p) ((p) = mem_calloc(1, sizeof *(p),\
				  __FILE__, __LINE__))
#define VNEW0(p, count, type) \
  ((p) = mem_calloc(1, (sizeof *(p) + (count) * sizeof(type)),		\
						__FILE__, __LINE__))

/* 释放p所指向的内存,将p置为NULL */
extern void mem_free(void *p,const char *file, int32_t line);
#define FREE(p) ((void) mem_free((p), __FILE__, __LINE__), (p) = NULL) /* p会被多次求值 */

extern void *mem_resize(void *p, int64_t nbytes, const char *file, int32_t line);
#define RESIZE(p, nbytes) ((p) = mem_resize((p), nbytes,  __FILE__, __LINE__)) /* p会被多次求值 */
