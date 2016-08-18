#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "assert.h"
#include "list.h"
#include "mem.h"

#define T List_T 

struct T {
    int64_t size; /* 节点个数 */
    
    struct node {
	struct node *next;	/* 下一个节点 */
	void *x;  /* 值 */
    } *first, *last; /* 首尾指针 */
};

/* 创建新链表 */
T list_new(void *x, ...)
{
     T list;
     NEW(list);
     list->size = 0;
     list->first = list->last = NULL;

     va_list ap;
     va_start(ap, x);

     struct node **pp; /* 二级指针 */
     
     /* 以NULL作为最后一个参数,表示参数结束.但链表不会保存NULL */
     for (pp = &list->first; x; x = va_arg(ap, void *)) {
	  NEW(*pp);
	  (*pp)->x = x;
	  list->size++;
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
     /* 创建新节点 */
     struct node *node;
     NEW(node);
     node->x = x;

     node->next = list->first;
     list->first = node;

     if (list_empty(list))
       list->last = node;

     list->size++;
}

/* 将x插入到list尾 */
void list_push_back(T list, void *x)
{
     /* 创建新节点 */
     struct node *node;
     NEW(node);
     node->x = x;
     node->next = NULL;
     
    if (list_empty(list))
       list->first = list->last = node;
    else { /* 插入到链表尾 */
       list->last->next = node;
       list->last = node;
     }

     list->size++;
}

/* 返回list首元素,并释放首节点 */
void *list_pop_front(T list)
{
     assert(!list_empty(list));

     struct node *next = list->first->next;
     void *x = list->first->x;
     FREE(list->first);
     list->first = next;
     list->size--;
     
     if (list_empty(list))
	  list->last = NULL;

     return x;
}

/* 返回list尾元素,并释放尾节点 */
void *list_pop_back(T list)
{
     assert(!list_empty(list));

     /* 找到倒数第二个节点 */
     struct node *prev = NULL;
     for (struct node *node = list->first;
	  node != list->last;
	  prev = node, node = node->next)
       ;
     
     void *x = list->last->x;
     FREE(list->last);
     list->last = prev;
     list->size--;
     
     if (list_empty(list))
	  list->first = NULL;

     return x;
}

/* 创建list的一个副本 */
T list_dup(T list)
{
     T list_copy = list_new(NULL); /* list_copy->last 已被置为NULL */
     list_copy->size = list->size;
     
     struct node **pp = &list_copy->first;
     for (struct node *node = list->first; node; node = node->next) {
	  NEW(*pp);
	  (*pp)->x = node->x;	/* 指向同一个对象 */
	  list_copy->last = *pp;
	  pp = &(*pp)->next;
     }

     *pp = NULL;

     return list_copy;
}

/* 将list_2链到list_1之后,返回list_1, 释放list_2*/
T list_merge(T list_1, T *list_2p)
{
     if (list_1->first == NULL) list_1->first = (*list_2p)->first;
     if (list_1->last != NULL) list_1->last->next = (*list_2p)->first;
     if ((*list_2p)->last != NULL) list_1->last = (*list_2p)->last;
     
     list_1->size += (*list_2p)->size;
     /* list_2的信息全部保存在list_1中,list_2可以被安全释放*/
     FREE(*list_2p);
     
     return list_1;
}

/* 逆转list中的元素 */
void list_reverse(T list)
{
     list->last = list->first;
     
     /* 逆转链表 */
     struct node *prev = NULL, *next;
     while (list->first) {
	  next = list->first->next;
	  list->first->next = prev;
	  prev = list->first;
	  list->first = next;
     }

     list->first = prev;
}

/* 对list中的每一个元素,应用apply函数,arg作为外界参数传入apply */
void list_traverse(T list, void apply(void **xp, void *arg), void *arg)
{
     assert(apply);

     for (struct node* node = list->first; node; node = node->next)
	  apply(&node->x, arg);
}

/* 创建一个n+1个元素的数组,每个数组元素保存链表中的一个元素,最后一个数组元素为end */
void **list_to_array(T list, void *end)
{
     /* array中的元素为'void *'类型 */
     void **array = MALLOC((list_size(list) + 1) * sizeof(*array)), **pp = array;
     /* 将链表中的元素,依次拷贝到array中 */
     for (struct node *node = list->first; node; node = node->next)
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
     return list_size(list) == 0;
}

/* 返回list中元素个数 */
int64_t list_size(T list)
{
    assert(list);
    return list->size;
}
