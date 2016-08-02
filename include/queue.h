#pragma once

#include <stdbool.h>

#define Queue_T T

typedef struct T *T;

extern T queue_new(void);
extern bool queue_empty(T);
extern void queue_push(T q, void *x);
extern void *queue_pop(T q);
extern void queue_free(T *q);

#undef T


