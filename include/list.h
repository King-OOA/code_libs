#pragma once

#include <stdint.h>
#include <stdbool.h>

#define T List_T 

typedef struct T *T;


/* 创建新链表,并返回 */
extern T list_new(void *x, ...); 
/* 将新元素x插入到list表头,返回list */
extern T list_prepend(T list, void *x);
/* 将新元素x插入到list尾,返回list */
extern T list_append(T list, void *x);
/* 将list首元素放入*x中,释放首节点,返回list */
extern T list_pop(T list, void **x);
/* 判断list是否为空链表 */
extern bool list_empty(T list);
/* 返回list中节点数目 */
extern int64_t list_length(T list);
/* 返回list的一个副本 */
extern T list_dup(T list);
/* 将list_2连接在list_1尾部,返回list_1,释放list_2 */
extern T list_cat(T list_1, T *list_2p);
/* 逆转list中的元素 */
extern T list_reverse(T list);
/* 对list中的每一个元素,执行apply函数,arg作为外界参数,传入apply函数 */
extern void list_traverse(T list, void apply(void **x, void *arg), void *arg);
/* 将list中的每个元素,依次放入一个数组,数组最后一个元素为end,返回该数组 */
extern void **list_to_array(T list, void *end);


#undef T
