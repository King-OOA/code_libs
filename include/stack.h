#pragma once

#include <stdbool.h>

#define Stack_T T

typedef struct T *T;

extern T stack_new(void);
extern bool stack_empty(T stk);
extern void stack_push(T stk, void *x);
extern void *stack_pop(T stk);
extern void stack_free(T *stk);

#undef T
