#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "assert.h"
#include "list.h"
#include "mem.h"

#define T List_T 


typedef struct node {
    struct node *next;	/* 下一个节点 */
    void *x;  /* 值 */
} *Node_T;

struct T {
    uint64_t len; /* 节点个数 */
    Node_T first, last; /* 首尾指针 */
};

/* 创建新链表 */
T list_new(void *x, ...)
{
    T list;
    NEW0(list);

    va_list ap;
    va_start(ap, x);

    Node_T *pp; /* 二级指针 */
     
    /* 以NULL作为最后一个参数,表示参数结束.但链表不会保存NULL */
    for (pp = &list->first; x; x = va_arg(ap, void *)) {
	NEW(*pp);
	(*pp)->x = x;
	list->len++;
	list->last = *pp;
	pp = &(*pp)->next;
    }

    *pp = NULL; /* 此时pp指向最后一个节点的next成员 */
    va_end(ap);
     
    return list;
}

/* 将x插入到list首 */
void list_push_front(T list, void *x)
{
    assert(list);
    
    /* 创建新节点 */
    Node_T node;
    NEW(node);
    node->x = x;

    node->next = list->first;
    list->first = node;

    if (list_empty(list))
	list->last = node;

    list->len++;
}

/* 将x插入到list尾 */
void list_push_back(T list, void *x)
{
    assert(list);
    
    /* 创建新节点 */
    Node_T node;
    NEW(node);
    node->x = x;
    node->next = NULL;
     
    if (list_empty(list))
	list->first = list->last = node;
    else { /* 插入到链表尾 */
	list->last->next = node;
	list->last = node;
    }

    list->len++;
}

/* 将x按由小到大的顺序插入到list */
void list_order_push(T list, void *x, int32_t compare(void const *x, void const *y))
{
    assert(list && compare);
    
    /* 创建新节点 */
    Node_T node;
    NEW(node);
    node->x = x;

    /* 找到合适的插入位置 */
    Node_T *pp;
    for (pp = &list->first;
	 *pp && compare((*pp)->x, node->x) < 0;
	 pp = &(*pp)->next)
	;
    
    /* 链表为空或者新节点将插入到链表尾*/
    if (*pp == NULL)
	list->last = node;
    
    /* 插入新节点 */
    node->next = *pp;
    *pp = node;

    list->len++;
}

/* 在list中搜索x,返回第一次出现的位置(节点);若不存在,返回NULL. x必须与链表元素类型相同 */
void *list_search(T list, void *x, int32_t compare(void const *x, void const *y))
{
    assert(list && compare);
    
    /* 顺序遍历链表寻找目标 */
    for (Node_T node = list->first; node; node = node->next)
	if (compare(node->x, x) == 0)
	    return node->x;

    return NULL;
}

/* 在有序list中搜索x,返回第一次出现的位置(节点);若不存在,返回NULL.x必须与链表元素类型相同 */
void *list_order_search(T list, void *x, int32_t compare(void const *x, void const *y))
{
    assert(list && compare);
    
    /* 顺序遍历链表寻找目标 */
    Node_T node;
    int32_t result = 1; /* 初始时没找到 */
    for (node = list->first;
	 node && (result = compare(node->x, x)) < 0;
	 node = node->next)
	;

    return (result == 0 ? node->x : NULL);
}

/* 返回list首元素,并释放首节点 */
void *list_pop_front(T list)
{
    assert(!list_empty(list));
    
    Node_T second = list->first->next;
    void *x = list->first->x;
    FREE(list->first);
    list->first = second;
    list->len--;
     
    if (list_empty(list))
	list->last = NULL;

    return x;
}

/* 返回list尾元素,并释放尾节点 */
void *list_pop_back(T list)
{
    assert(!list_empty(list));
    
    /* 找到倒数第二个节点 */
    Node_T prev = NULL;
    for (Node_T node = list->first;
	 node != list->last;
	 prev = node, node = node->next)
	;
     
    void *x = list->last->x;
    FREE(list->last);
    list->last = prev;
    list->len--;
     
    if (list_empty(list))
	list->first = NULL;

    return x;
}

/* 创建list的一个副本 */
T list_dup(T list)
{
    assert(list);
    
    T copy = list_new(NULL); /* copy->last 已被置为NULL */
    copy->len = list->len;
     
    Node_T *pp = &copy->first;
    for (Node_T node = list->first; node; node = node->next) {
	NEW(*pp);
	(*pp)->x = node->x;	/* 指向同一个对象 */
	copy->last = *pp;
	pp = &(*pp)->next;
    }

    *pp = NULL;

    return copy;
}

/* 将list_2链到list_1之后,返回list_1, 保留list_2*/
/* 语义尚不明确*/
/* T list_append(T list_1, T list_2) */
/* { */
/*     assert(list_1 && list_2); */
    
/*     if (list_1->first == NULL) list_1->first = list_2->first; */
/*     if (list_1->last != NULL) list_1->last->next = list_2->first; */
/*     if (list_2->last != NULL) list_1->last = list_2->last; */
     
/*     list_1->len += list_2->len; */
/*     /\* list_2的信息全部保存在list_1中,list_2可以被安全释放*\/ */
/*     FREE(list_2); */
     
/*     return list_1; */
/* } */

/* 逆转list中的元素 */
void list_reverse(T list)
{
    assert(list);

    list->last = list->first;
     
    /* 逆转链表 */
    Node_T prev = NULL, next;
    while (list->first) {
	next = list->first->next;
	list->first->next = prev;
	prev = list->first;
	list->first = next;
    }

    list->first = prev;
}

/* 对list中的每一个元素,应用apply函数,arg作为外界参数传入apply */
/* 可并行 */
void list_apply(T list, void apply(void **xp, void *arg), void *arg)
{
    assert(list && apply);

    for (Node_T node = list->first; node; node = node->next)
	apply(&node->x, arg);
}

/* 创建一个n+1个元素的数组,每个数组元素保存链表中的一个元素,最后一个数组元素为end */
void **list_to_array(T list, void *end)
{
    assert(list);
    
    /* array中的元素为'void *'类型 */
    void **array = MALLOC(list_len(list) + 1, void *), **pp = array;
    /* 将链表中的元素,依次拷贝到array中 */
    for (Node_T node = list->first; node; node = node->next)
	*pp++ = node->x;
    /* array的最后一个元素为end */
    *pp = end;

    return array;
}

/* 销毁链表及其节点,但不销毁节点所指向的元素 */
void list_free(T *list)
{
    assert(list && *list);
    /* 销毁每个节点 */
    while ((*list)->first) {
	(*list)->last = (*list)->first->next;
	FREE((*list)->first);
	(*list)->first = (*list)->last;
    }
    /* 销毁链表结构 */
    FREE(*list);
}

/* 判断链表是否为空 */
bool list_empty(T list)
{
    return list_len(list) == 0;
}

/* 返回list中元素个数 */
int64_t list_len(T list)
{
    assert(list);
    return list->len;
}
