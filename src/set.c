#include "stdhs.h"
#include "mem.h"
#include "set.h"
#include "arith.h"

#define T Set_T

/* 为集合set复制一个新集合 */
#define set_copy(set, hint) hashmap_copy((set), (hint))
extern T hashmap_copy(T hashmap, int32_t hint);

/* 比较集合s和t的cmp及hash是否都相同 */
#define same_funs(s, t) hashmap_same_funs((s), (t))
extern bool hashmap_same_funs(T s, T t);

/* 取得集合set的cmp函数 */
#define set_cmp(set)  hashmap_get_cmp((set))
extern int (*hashmap_get_cmp(T hashmap)) (void const *, void const *);

/* 取得集合set的hash函数 */
#define set_hash(set)  hashmap_get_hash((set))
extern uint64_t (*hashmap_get_hash(T hashmap)) (void const *);


void **set_to_array(T set, void *end)
{
    void **array1 = hashmap_to_array(set, NULL);
    
    int64_t n = set_volume(set);
    void **array2 = MALLOC(n + 1, void *);
    
    for (int64_t i = 0; i < n; i++)
	array2[i] = array1[2*i];

    array2[n] = end;
    FREE(array1);

    return array2;
}

static void apply_union_put(void const *key, void **null, void *union_set)
{
    set_put(union_set, (void *) key);
}

T set_union(T s, T t)
{
    if (s == NULL) {
	assert(t);
	return set_copy(t, set_volume(t));
    } else if (t == NULL) 
	return set_copy(s, set_volume(s));
    else {
	assert(same_funs(s, t));
	T union_set = set_copy(s, arith_max(set_volume(s), set_volume(t)));
	/* 将t中的每个元素放入union_set中 */
	set_traverse(t, apply_union_put, union_set);

	return union_set;
    }
}

static void apply_inter_put(void const *key, void **null, void *sets)
{
    if (set_belong(((T *)sets)[0], key))
	set_put(((T *)sets)[1], (void *)key);
}

T set_inter(T s, T t)
{
    if (s == NULL) {
	assert(t); /* 新的空集 */
	return set_new(set_volume(t), set_cmp(t), set_hash(t));
    } else if (t == NULL) /* 新的空集 */
	return set_new(set_volume(s), set_cmp(s), set_hash(s));
    else {
	assert(same_funs(s, t));
	int64_t s_volume = set_volume(s);
	int64_t t_volume = set_volume(t);

	T smaller, bigger;
	if (s_volume < t_volume)
	    smaller = s, bigger = t;
	else 
	    smaller = t, bigger = s;
	
	T inter_set = set_new(arith_min(s_volume, t_volume),
			  set_cmp(t), set_hash(t));
	/* 将在smaller中同时也在bigger中的元素放入inter_set中 */
	T sets[2] = {bigger, inter_set};
	set_traverse(smaller, apply_inter_put, sets);

	return inter_set;
    }
}

static void apply_minus_put(void const *key, void **null, void *sets)
{
    if (!set_belong(((T *)sets)[0], key))
	set_put(((T *)sets)[1], (void *)key);
}

T set_minus(T s, T t)
{
    if (s == NULL) {
	assert(t); /* 返回一个新的空集 */
	return set_new(set_volume(t), set_cmp(t), set_hash(t));
    } else if (t == NULL) 
	return set_copy(s, set_volume(s));
    else {
	assert(same_funs(s, t));
	T minus_set = set_new(arith_min(set_volume(s),set_volume(t)),
			      set_cmp(s), set_hash(s));
	T sets[2] = {t, minus_set};
	/* 将属于s但不属于t的元素放入minus_set中 */
	set_traverse(s, apply_minus_put, sets);

	return minus_set;
    }
}

T set_diff(T s, T t)
{
    if (s == NULL) {
	assert(t);
	return set_copy(t, set_volume(t));
    } else if (t == NULL) 
	return set_copy(s, set_volume(s));
    else {
	assert(same_funs(s, t));
	T diff_set = set_new(arith_min(set_volume(s), set_volume(t)),
			     set_cmp(s), set_hash(s));
	T sets[2] = {t, diff_set};
	/* 将属于s但不属于t的元素放入diff_set中 */
	set_traverse(s, apply_minus_put, sets);
	sets[0] = s;
	/* 再将属于t但不属于s的元素放入diff_set中 */
	set_traverse(t, apply_minus_put, sets);

	return diff_set;
    }
}

