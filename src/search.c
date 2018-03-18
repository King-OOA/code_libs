#include "stdhs.h"
#include "search.h"



/* 线性查找, 数组无序 */
void *
linear_search(void *base, size_t num, size_t size, int (*compare)(const void *, const void *), void *key)
{
    for (char *end = (char *) base + num*size; (char *) base < end; base = (char *) base + size)
	if (compare(base, key) == 0)
	    return base;

    return NULL;
}


/* 二分查找, 数组有序 */
void *
binary_search(void *base, size_t num, size_t size, int (*compare)(const void *, const void *), void *key)
{
  int64_t low = 0, high = num - 1;

  while (low <= high) {
    int64_t mid = (high + low) >> 1;
    char *mid_e = (char *) base + mid*size;

    if (compare(key, mid_e) == 0)
      return mid_e;
    else if (compare(key, mid_e) < 0)
      high = mid - 1;
    else
      low = mid + 1;
  }

  return NULL;
}  
