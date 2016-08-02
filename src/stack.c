#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "assert.h"
#include "mem.h"
#include "stack.h"

#define T Stack_T

struct T {
    int64_t count; /* 元素个数 */
    struct elem {
	void *x;
	struct elem *next;
    } *head;
};

T stack_new(void)
{
    T stk;

    NEW(stk);
    stk->count = 0;
    stk->head = NULL;

    return stk;
}

bool stack_empty(T stk)
{
    assert(stk);
    return stk->count == 0;
}

void stack_push(T stk, void *x)
{
    struct elem *t;

    assert(stk);

    NEW(t);
    t->x = x;
    t->next = stk->head;
    stk->head = t;
    stk->count++;
}

void *stack_pop(T stk)
{
    assert(stk);
    assert(stk->count > 0);

    struct elem *t = stk->head;
    stk->head = t->next;
    stk->count--;
    void *x = t->x;
    FREE(t);
  
    return x;
}

void stack_free(T *stk)
{
    assert(stk && *stk);

    for (struct elem *t = (*stk)->head, *u; t; t = u) {
	u = t->next;
	FREE(t);
    }
  
    FREE(*stk);
}
