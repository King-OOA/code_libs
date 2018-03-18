#pragma once
#include <stdint.h>


#define T Array_T
typedef struct T *T;


/* 创建新的数组对象,为其分配空间并初始化.
   数组包含capacity个元素, 每个元素大小为size字节 */
extern T array_new_sz(int64_t capacity, int64_t size);
/* 用元素类型作为参数 */
#define array_new(capacity, type) array_new_sz((capacity), capacity*sizeof(type))

/* 释放*array_p所指的数组(参数为二级指针) */
extern void array_free(T *array);

/* 返回array中的元素个数 */
extern int64_t array_capacity(T array);

/* 返回array中每个元素的大小 */
extern int64_t array_esize(T array);

/* 将e所指向的元素,拷贝到array的第i个位置,返回e */
extern void *array_put(T array, int64_t i, void *e);

/* 返回指向array第i个元素的指针(并不删除该元素) */
extern void *array_get(T array, int64_t i);

/* 改变array中的元素数目(收缩或扩展) */
extern void array_resize(T array, int64_t capacity);

/* 将array中前capacity个元素复制到新的数组中,返回新数组对象
 (若capacity大于array元素个数, 则多余的元素被清零)*/
extern T array_copy(T array, int64_t capacity);

#undef T 
