#pragma once

#include "stdhs.h"

#define T Array_T


struct T {
    int64_t capacity;
    int64_t size;
    char *buf;
};


/* 初始化数组各个字段(不分配任何空间) */
extern void array_init(T array, int64_t capacity, int64_t size, void *buf);

#undef T
