#pragma once

#include <stdint.h>
#include <stdbool.h>

#define CIRCULAR 1

#define T List_T


typedef struct T *T;


/* 创建新链表,并返回.可以在创建的同时,传入参数(指向对象的指针),以NULL指针作为最后一个参数 */
extern T list_new(void *x, ...); 
/* 销毁链表及其节点,但不销毁节点所指向的元素 */
extern void list_free(T *list);

/* 将x插入到list首 */
extern void list_push_front(T list, void *x);
/* 将x插入到list尾 */
extern void list_push_back(T list, void *x);
/* 将x按由小到大的顺序插入到list */
extern void list_order_push(T list, void *x, int32_t compare(const void *x, const void *y));

/* 在list中搜索x,返回第一次出现的位置(节点);若不存在,返回NULL */
extern void *list_search(T list, void *x, int32_t compare(void const *x, void const *y));
/* 在有序list中搜索x,返回第一次出现的位置(节点);若不存在,返回NULL */
extern void *list_order_search(T list, void *x, int32_t compare(void const *x, void const *y));

/* 返回list首元素,并释放首节点 */
extern void *list_pop_front(T list);
/* 返回list尾元素,并释放尾节点 */
extern void *list_pop_back(T list);

/* 判断list是否为空 */
extern bool list_empty(T list);
/* 返回list中节点个数 */
extern int64_t list_len(T list);

/* 返回list的一个副本 */
extern T list_dup(T list);

/* 将list_2连接在list_1尾部,返回list_1,保留list_2 */
//extern T list_merge(T list_1, T list_2);

/* 逆转list中的元素 */
extern void list_reverse(T list);
/* 对list中的每一个元素,执行apply函数,arg作为外界参数,传入apply函数 */
extern void list_apply(T list, void apply(void **xp, void *arg), void *arg);
/* 将list中的每个元素,依次放入一个数组,数组最后一个元素为end(通常为
   NULL),返回该数组 */
extern void **list_to_array(T list, void *end);


#undef T
