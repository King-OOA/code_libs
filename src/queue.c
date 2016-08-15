#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "assert.h"
#include "queue.h"
#include "mem.h"

#define T Queue_T

struct T {
  int64_t count; /* 元素个数 */

  struct elem {
    struct elem *next;
    void *x;
  } *head, *tail;
};

T queue_new(void)
{
  T q;

  NEW(q);
  q->count = 0;
  q->tail = q->head = NULL;
  
  return q;
}

bool queue_empty(T q)
{
  assert(q);
  return q->count == 0;
}

void queue_push(T q, void *x)
{
  struct elem *t;

  assert(q);

  NEW(t);
  t->x = x;
  t->next = NULL;
  
  if (Queue_empty(q))
    q->head = q->tail = t;
  else {
    q->tail->next = t;
    q->tail = t;
  }
  
  q->count++;
}

void *queue_pop(T q)
{
  assert(q);
  assert(q->count > 0);

  struct elem *t = q->head;
  q->head = t->next;
  q->count--;
  void *x = t->x;
  FREE(t);
  
  if (Queue_empty(q))
    q->tail = NULL;
  
  return x;
}

void queue_free(T *q)
{
  assert(q && *q);
  
  for (struct elem *t = (*q)->head, *u; t; t = u) {
    u = t->next;
    FREE(t);
  }

  FREE(*q);
}
