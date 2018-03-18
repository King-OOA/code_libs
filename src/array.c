#include "stdhs.h"
#include "mem.h"
#include "array.h"
#include "array_low.h"

#define T Array_T

/* 创建新的数组对象,为其分配空间并初始化.
   数组包含num个元素, 每个元素大小为size字节 */
T array_new_sz(int64_t capacity, int64_t size)
{
    T array;
    NEW(array);

    if (capacity > 0)
	array_init(array, capacity, size, CALLOC(capacity, size));
    else 
	array_init(array, capacity, size, NULL);

    return array;
}

/* 初始化数组各个字段(不分配任何空间) */
void array_init(T array, int64_t capacity, int64_t size, void *buf)
{
    assert(array);
    assert((buf && capacity > 0) || (capacity == 0 && buf == NULL));
    assert(size > 0);

    array->capacity = capacity;
    array->size = size;
    
    if (capacity > 0)
	array->buf = buf;
    else
	array->buf = NULL;
}

/* 释放*array_p所指的数组(参数为二级指针) */
void array_free(T *array_p)
{
    assert(array_p && *array_p);

    FREE((*array_p)->buf);
    FREE(*array_p);
}

/* 返回指向array第i个元素的指针(并不删除该元素) */
void *array_get(T array, int64_t i)
{
    assert(array);
    assert(i >= 0 && i < array->capacity);
    
    return array->buf + i * array->size;
}

/* 将elem所指向的元素,拷贝到array的第i个位置,返回elem */
void *array_put(T array, int64_t i, void *elem)
{
    assert(array);
    assert(i >= 0 && i < array->capacity);
    assert(elem);

    memcpy(array->buf + i * array->size, elem, array->size);

    return elem;
}


/* void *array_insert(T array, int64_t i, void *elem) */
/* { */
/*    assert(array); */
/*    assert(i >= 0 && i < array->capacity); */
/*    assert(elem); */

/*    /\* 先扩容 *\/ */
/*    array_resize(array, array->capacity + 1); */

/*    /\* 再腾出空间 *\/ */
/*    char *src = array->buf + i*array->size; */
/*    char *dest = src + array->size; */
/*    memmove(dest, src, array->size); */
   
/*    /\* 最后插入 *\/ */
/*    memcpy(src, elem, array->size); */
   
/*    array->capacity++; */
   
/*    return elem; */
/* } */

/* 返回array中的元素个数 */
int64_t array_capacity(T array)
{
    assert(array);
    
    return array->capacity;
}

/* 返回array中每个元素的大小 */
int64_t array_esize(T array)
{
    assert(array);
    
    return array->size;
}

/* 改变array中的元素数目(收缩或扩展) */
void array_resize(T array, int64_t capacity)
{
    assert(array);
    assert(capacity >= 0);

    if (capacity == 0)
	FREE(array->buf);
    else if (array->capacity == 0)
	array->buf /*此时,array->buf为NULL, RESIZE将报错  */
	    = mem_alloc(capacity*array->size, __FILE__, __LINE__);
    else
	RESIZE(array->buf, capacity*array->size);
    
    array->capacity = capacity;
}

/* 将array中前capacity个元素复制到新的数组中,返回新数组对象
 (若num大于array元素个数, 则多余的元素被清零)*/
T array_copy(T array, int64_t capacity)
{
    assert(array);
    assert(capacity >= 0);

    T new_array;
    new_array = array_new(capacity, array->size);

    /* 取capacity和array->capacity中较小的 */
    if (capacity >= array->capacity && array->capacity > 0)
	memcpy(new_array->buf, array->buf, array->capacity*array->size);
    else if (capacity < array->capacity && capacity > 0)
	memcpy(new_array->buf, array->buf, capacity*array->size);

    return new_array;
}
