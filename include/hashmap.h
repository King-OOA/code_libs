#pragma once
#include <stdint.h>

#define T Hashmap_T

typedef struct T *T;

/* 创建新的哈希表, bucket_num一经设定不可改变.  */
extern T hashmap_new(int64_t hint,
		     int32_t cmp_key(void const *x, void const *y),
		     uint64_t hash(void const *key));


/* 释放hashmap的所有表项(并不释放键或值)及hashmap本身. */
extern void hashmap_free(T *hashmap_p);

/* 返回hashmap中的键值对数量 */
extern int64_t hashmap_volume(T hashmap);

/* 将key-value键值对加入hashmap.
   若key已存在, 则value将覆盖key原先的value, 返回原先的value;
   否则, 新建表项以容纳该键值对, 返回NULL. */
extern void *hashmap_put(T hashmap, void const *key, void *value);

/* 在hashmap中搜索key, 若key存在, 则返回key所对应的value. 否则, 返回NULL. */
extern void *hashmap_get(T hashmap, void const *key);

/* 在hashmap中搜索key, 若key存在,用free_key函数释放键, 同时释放
 * key-value键值对, 返回key所对应的value(不删除); 否则, 不做任何修改,
 * 返回NULL. */
extern void *hashmap_remove(T hashmap, void const *key, void free_key(void *key));

/* 遍历hashmap中的每个元素(键值对), 对其执行apply函数, arg作为外界参数传入apply函数. */
extern void hashmap_traverse(T hashmap,
			     void apply(void const *key, void **value, void *arg),
			     void *arg);

/* 将hashmap中的每个键值对(假设有N个)收集起来, 放入一个有2N+1个元素的
 * 数组中. 键存放于数组的偶数下标处, 对应的值存放于下一个奇数下标处.
 * 数组最后一个元素用end标示(通常为NULL). 返回该数组.*/
extern void **hashmap_to_array(T hashmap, void *end);

/* 释放每个node(但并不释放key和value),然后释放整个哈希表 */
extern void hashmap_free(T *hashmap_p);

#undef T
