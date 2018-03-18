/***********************************************************************
****  作者:彭展                                 
****                                           
****  序列的元素均为指针.                      
****                                          
****  主要支持操作:                           
****   1. O(1)时间访问或更改序列中的任意元素.  
****   2. 在序列首/尾添加或删除元素.(在序列尾添加或删除元素比较高效)
****					       
****  不支持的操作:                            
****   在序列中的任意位置添加或删除元素         
***********************************************************************/


#pragma once
#include <stdint.h>
#include <stdbool.h>

#define T Seq_T

typedef struct T *T;

/* 创建长为len的新序列 */
extern T seq_new(int64_t len);

/* 用可变参数来构造序列, 以NULL结尾 */
extern T seq_seq(void *e1, ...);

/* 销毁序列(参数为指针的指针) */
extern void seq_free(T *seq_p);

/* 返回序列长度 */
extern int64_t seq_len(T seq);

/* 序列为空时, 返回true */
extern bool seq_empty(T seq);

/* 返回序列的第i个元素(并不删除) */
extern void *seq_get(T seq, int64_t i);

/* 将序列第i个元素替换为e, 返回序列原先的值 */
extern void *seq_put(T seq, int64_t i, void *e);

/* 将元素e插入到序列首,返回e */
extern void *seq_push_front(T seq, void *e);

/* 将元素e插入到序列尾,返回e */
extern void *seq_push_back(T seq, void *e);

/* 返回序列的首元素(第0个),同时将其删除 */
extern void *seq_pop_front(T seq);

/* 返回序列的尾元素(第seq->len-1个),同时将其删除 */
extern void *seq_pop_back(T seq);

#undef T
