#include "stdhs.h"
#include "seq.h"
#include "array.h"
#include "array_low.h"
#include "mem.h"

#define T Seq_T

struct T {
    struct Array_T array; /* 序列的第一个字段, 确保序列和数组的地址相同 */
    int64_t head; /* 序列首元素所在数组下标(起始位置) */
    int64_t len; /* 序列所包含元素的数目(偏移量) */
};

#define INIT_LEN 16 /* 当创建的序列长为0时, 为其数组分配的初始容量*/

/* 创建长为len的新序列 */
T seq_new(int64_t len)
{
    assert(len >= 0);

    T seq; NEW0(seq);

    /* 若长为0, 则初始分配INIT_LEN个指针 */
    if (len == 0)
	len = INIT_LEN;
    
    array_init(&seq->array, len, sizeof(void *),
	       MALLOC(len, void *));

    return seq;
}

/* 用可变参数来构造序列, 以NULL结尾 */
T seq_seq(void *e, ...)
{
    T seq = seq_new(0);

    va_list ap;

    for (va_start(ap, e); e; e = va_arg(ap, void *))
	seq_push_back(seq, e);

    va_end(ap);
    
    return seq;
}

/* 销毁序列(参数为指针的指针) */
void seq_free(T *seq_p)
{
    assert(seq_p && *seq_p);
    /* 确保序列及其内嵌数组的地址相同 */
    assert((void *)*seq_p == (void *)&(*seq_p)->array);
    /* 释放数组的同时,也是放了序列 */
    array_free((Array_T *)seq_p);
}

/* 返回序列长度 */
int64_t seq_len(T seq)
{
    assert(seq);

    return seq->len;
}

/* 序列为空时, 返回true */
bool seq_empty(T seq)
{
    return seq->len == 0;
}

/* 序列的第i个元素(i在宏里只计算一次) */
#define seq_e(i) \
    ((void **)seq->array.buf)[(seq->head + (i)) % seq->array.capacity]


/* 返回序列的第i个元素(并不删除) */
void *seq_get(T seq, int64_t i)
{
    assert(seq);
    assert(i >=0 && i < seq->len);

    return seq_e(i);
}

/* 将序列第i个元素替换e, 返回原值 */
void *seq_put(T seq, int64_t i, void *e)
{
    assert(seq);
    assert(i >=0 && i < seq->len);

    void *old = seq_e(i);
    seq_e(i) = e;

    return old;
}

/* 返回序列的首元素(第0个),同时将其删除 */
void *seq_pop_front(T seq)
{
    assert(seq);
    assert(seq->len > 0);

    void *e = seq_e(0);
    seq->head = (seq->head + 1) % seq->array.capacity;
    seq->len--;
    
    return e;
}

/* 返回序列的尾元素(第seq->len-1个),同时将其删除 */
void *seq_pop_back(T seq)
{
    assert(seq);
    assert(seq->len > 0);
    
   return seq_e(--seq->len);
}

/* 将seq的数组容量增倍 */
static void expand(T seq)
{
    int64_t n = seq->array.capacity;
    array_resize(&seq->array, 2*n);
    
    if (seq->head > 0) {
	void **head = (void **) seq->array.buf + seq->head;
	memcpy(head + n, head,
	       (n - seq->head) * sizeof(void *));
	seq->head += n;
    }
 }

/* 将元素e插入到序列首,返回e */
void *seq_push_front(T seq, void *e)
{
    assert(seq);

    /* 容量已满,则先扩容 */
    if (seq->len == seq->array.capacity)
	expand(seq);

    if (--seq->head < 0)
	seq->head = seq->array.capacity - 1;

    seq->len++;
    
    return seq_e(0) = e;
}

/* 将元素e插入到序列尾,返回e */
void *seq_push_back(T seq, void *e)
{
    assert(seq);

    /* 容量已满,则先扩容 */
    if (seq->len == seq->array.capacity)
	expand(seq);
    
    return seq_e(seq->len++) = e;
}


