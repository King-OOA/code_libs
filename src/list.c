#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "assert.h"
#include "list.h"
#include "mem.h"

#define T List_T 

struct T {
    int64_t count; /* 节点个数 */
    
    struct node {
	struct node *next;
	void *x;
    } *first, *last; /* 首尾指针 */
};

T list_new(void *x, ...)
{
     T list;
     NEW(list);
     list->count = 0;
     list->first = list->last = NULL;

     va_list ap;
     va_start(ap, x);

     struct node **p;
     for (p = &list->first; x; x = va_arg(ap, void *)) {
	  NEW(*p);
	  (*p)->x = x;
	  list->count++;
	  list->last = *p;
	  p = &(*p)->next;
     }

     *p = NULL;
     va_end(ap);
     
     return list;
}

/* 插入到链表头 */
T list_prepend(T list, void *x)
{
     struct node *t;
     /* 插入到链表头 */
     NEW(t);
     t->x = x;
     t->next = list->first;
     list->first = t;
     list->count++;
    /* 若是链表的第一个元素,则为链表尾 */
     if (list_empty(list))
	  list->last = t;

     return list;
}

/* 插入到链表尾 */
T list_append(T list, void *x)
{
     struct node *t;
     /* 插入到链表尾 */
     NEW(t);
     t->x = x;
     t->next = NULL;
     list->last = t;
     list->count++;
    /* 若是链表的第一个元素,则为链表头 */
     if (list_empty(list))
	  list->first = t;

     return list;
}

/* 将list第一个节点的元素保存在*xp中,删除第一个节点,返回list */
T list_pop(T list, void **xp)
{
     assert(!list_empty(list));

     struct node *second = list->first->next;

     *xp = list->first->x;
     FREE(list->first);
     list->first = second;
     list->count--;
     
     if (list_empty(list))
	  list->last = NULL;

     return list;
}

T list_dup(T list)
{
     T new_list = list_new(NULL);
     struct node **p = &new_list->first;
     
     for (struct node *node = list->first; node; node = node->next) {
	  NEW(*p);
	  (*p)->x = node->x;
	  new_list->last = *p;
	  p = &(*p)->next;
     }

     *p = NULL;
     new_list->count = list->count;

     return new_list;
}

T list_cat(T list_1, T *list_2p)
{
     if (list_1->first == NULL) list_1->first = (*list_2p)->first;
     if (list_1->last != NULL) list_1->last->next = (*list_2p)->first;
     if ((*list_2p)->last != NULL) list_1->last = (*list_2p)->last;
     
     list_1->count += (*list_2p)->count;
     
     FREE(*list_2p);
     
     return list_1;
}

T list_reverse(T list)
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
     
     return list;
}

void list_traverse(T list, void apply(void **x, void *arg), void *arg)
{
     assert(apply);

     for (struct node* node = list->first; node; node = node->next)
	  apply(node->x, arg);
}

void **list_to_array(T list, void *end)
{
     /* array中的元素为'void *'类型 */
     void **array = MALLOC((list_length(list) + 1) * sizeof(*array)), **p = array;
     /* 将链表中的元素,依次拷贝到array中 */
     for (struct node *node = list->first; node; node = node->next)
	  *p++ = node->x;
     /* array的最后一个元素为end */
     *p = end;

     return array;
}

void list_free(T *list)
{
     assert(list);
     /* 销毁节点 */
     while ((*list)->first) {
	  (*list)->last = (*list)->first->next;
	  FREE((*list)->first);
	  (*list)->first = (*list)->last;
     }
     /* 销毁链表结构 */
     FREE(*list);
}


bool list_empty(T list)
{
     return list->count == 0;
}

int64_t list_length(T list)
{
     return list->count;
}
