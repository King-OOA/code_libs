#pragma once

#include "list.h"
#include "seq.h"

#define IMPLEMENTATION SEQ


/* #if (IMPLEMENTATION == LIST) */

/* /\* 栈用链表实现 *\/ */
/* typedef List_T Stack_T;  */

/* #define stack_new() list_new(NULL) */
/* #define stack_push(stk, x) list_push_front((stk), (x)) */
/* #define stack_pop(stk) list_pop_front((stk)) */
/* #define stack_empty(stk) list_empty((stk)) */
/* #define stack_size(stk) list_len((stk)) */
/* #define stack_free(stkp) list_free((stkp)) */

/* /\* 队列用链表实现 *\/ */
/* typedef List_T Queue_T;  */

/* #define queue_new() list_new(NULL) */
/* #define queue_push(q, x) list_push_back((q), (x)) */
/* #define queue_pop(q) list_pop_front((q)) */
/* #define queue_empty(q) list_empty((q)) */
/* #define queue_size(q) list_len((q)) */
/* #define queue_free(qp) list_free((qp)) */

/* #else */

/* 栈用序列(数组)实现 */
typedef Seq_T Stack_T; 

#define stack_new() seq_new(0)
#define stack_push(stk, x) seq_push_back((stk), (x))
#define stack_pop(stk) seq_pop_back((stk))
#define stack_empty(stk) seq_empty((stk))
#define stack_size(stk) seq_len((stk))
#define stack_free(stkp) seq_free((stkp))

/* 队列用序列(数组)实现 */
typedef Seq_T Queue_T; 

#define queue_new() seq_new(0)
#define queue_push(q, x) seq_push_back((q), (x))
#define queue_pop(q) seq_pop_front((q))
#define queue_empty(q) seq_empty((q))
#define queue_size(q) seq_len((q))
#define queue_free(qp) seq_free((qp))

//#endif 
