#include "stdhs.h"

/* 二分查找 */
void *binary_search(void *base, size_t num, size_t size, int (*compare)(const void *, const void *), void *key);

/* 线性查找 */
void *linear_search(void *base, size_t num, size_t size, int (*compare)(const void *, const void *), void *key);


