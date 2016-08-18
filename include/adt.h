#pragma once

#include "list.h"

/* 栈用链表实现 */
typedef List_T Stack_T; 

#define stack_new() list_new(NULL)
#define stack_push(stk, x) list_push_front((stk), (x))
#define stack_pop(stk) list_pop_front((stk))
#define stack_empty(stk) list_empty((stk))
#define stack_size(stk) list_size((stk))
#define stack_free(stkp) list_free((stkp))

/* 队列用链表实现 */
typedef List_T Queue_T; 

#define queue_new() list_new(NULL)
#define queue_push(q, x) list_push_back((q), (x))
#define queue_pop(q) list_pop_front((q))
#define queue_empty(q) list_empty((q))
#define queue_size(q) list_size((q))
#define queue_free(qp) list_free((qp))
