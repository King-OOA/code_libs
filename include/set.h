#pragma once

#include "hashmap.h"

/* 用哈希表实现集合, 这样当哈希表的底层实现发生变化时,上层集合接口的变
   * 化将尽可能小. */

/* 注:这里的空集定义为不包含任何元素的集合实体。NULL不是空集。 */


typedef Hashmap_T Set_T;
#define T Set_T

/* 创建新集合 */
#define set_new(hint, cmp, hash) hashmap_new((hint), (cmp), (hash))

/* 判断member是否属于set */
#define set_belong(set, member) (hashmap_get((set), (member)) != NULL)

/* 将成员member添加到set中. 如果member已经存在, 则不做任何改变 */
/* 键和值都指向同一个元素. 注意:值不能为NULL, 否则无法区分hashmap_get
 * 的返回值*/
#define set_put(set, member)  hashmap_put((set), (member), (member))

/* 释放集合中所有元素节点(不释放元素本身)和集合本身 */
#define set_free(set) hashmap_free((set))

/* 返回集合中元素的个数 */
#define set_volume(set) hashmap_volume((set))

/* 若member属于set, 则将其删除, 返回member; 否则, 不做任何更改, 返回
 * NULL. */
#define set_remove(set, member) hashmap_remove((set), (member), NULL)

/* 遍历集合中的元素. 注意: apply函数必须与hashmap_traverse中的apply参
 * 数相同,即: void apply(void const *member, void const **useless,
 * void *arg)其中第二个参数无用 */
#define set_traverse(set, apply, arg) hashmap_traverse((set), (apply), (arg))
    
/* 将集合中的元素构建成数组 */
void **set_to_array(T set, void *end);

/* 返回集合s和t的并集. s和t本身不变 */
T set_union(T s, T t);

/* 返回集合s和t的交集. s和t本身不变 */
T set_inter(T s, T t);

/* 返回集合s和t的差集(s-t). s和t本身不变 */
T set_minus(T s, T t);

/* 返回集合s和t的对称差. s和t本身不变 */
T set_diff(T s, T t);


#undef T
