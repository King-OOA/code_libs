#include "stdhs.h"
#include "hashmap.h"
#include "mem.h"

#define T Hashmap_T

struct T {
    int64_t bucket_num; /*桶的数量  */
    int64_t node_num; /* 包含节点的数量 */
    uint64_t timestap; /* 时间戳.任何修改哈希表的操作都会导致其变化 */
    int (*cmp_key) (void const *x, void const *y);
    uint64_t (*hash) (void const *x);

    struct node {
	struct node *next;
	void const *key;
	void *value;
    } *buckets[];
};


static int32_t cmp_atom(void const *x, void const *y)
{
    return x != y;
}

static uint64_t hash_atom(void const *key)
{
    return (uint64_t) key >> 2;
}

T hashmap_new(int64_t hint, int cmp_key(void const *x, void const *y),
	      uint64_t hash(void const *key))
{
    assert(hint >= 0);

    static int64_t primes[] = {509, 509, 1021, 2053, 4093, 8191,
			       16381, 32771, 65521, INT64_MAX};
    int64_t bucket_num;
    for (int64_t i = 0; primes[i] < hint; i++)
	bucket_num = primes[i];
    
    T hashmap;
    VNEW0(hashmap, bucket_num, struct node *);
    /* 只需初始化以下3个成员，其它成员已被初始化为0 */
    hashmap->bucket_num = bucket_num;
    hashmap->cmp_key = cmp_key ? cmp_key : cmp_atom;
    hashmap->hash = hash ? hash : hash_atom;
  
    return hashmap;
}

void *hashmap_get(T hashmap, void const *key)
{
    assert(hashmap);
    assert(key);

    uint64_t i = hashmap->hash(key) % hashmap->bucket_num;
  
    for (struct node *p = hashmap->buckets[i]; p; p = p->next)
	if (hashmap->cmp_key(key, p->key) == 0)
	    return p->value;
	      
    return NULL;  
}

void *hashmap_put(T hashmap, void const *key, void *value)
{
    assert(hashmap);
    assert(key);

    uint64_t i = hashmap->hash(key) % hashmap->bucket_num;

    struct node *p;
    for (p = hashmap->buckets[i]; p; p = p->next)
	if (hashmap->cmp_key(key, p->key) == 0) {
	    void *old_value = p->value;
	    p->value = value;
	    return old_value;
	}
  
    NEW(p);
    p->key = key;
    p->value = value;
    p->next = hashmap->buckets[i];
    hashmap->buckets[i] = p;
    hashmap->node_num++;
    hashmap->timestap++;
  
    return NULL;
}

void *hashmap_remove(T hashmap, void const *key, void free_key(void *key))
{
    assert(hashmap);
    assert(key);

    uint64_t i = hashmap->hash(key) % hashmap->bucket_num;

    for (struct node **next_p = &hashmap->buckets[i];
	 *next_p; next_p = &(*next_p)->next)
	if (hashmap->cmp_key((*next_p)->key, key) == 0) {
	    struct node *node = *next_p;
	    void *value = node->value;
	    *next_p = node->next;
	    if (free_key)
		free_key((void *)node->key);
	    FREE(node);
	    hashmap->node_num--;
	    hashmap->timestap++;
	    return value;
	}

    return NULL;
}

int64_t hashmap_volume(T hashmap)
{
    assert(hashmap);

    return hashmap->node_num;
}

void hashmap_traverse(T hashmap, void apply(void const *key, void **value, void *arg),
		      void *arg)
{
    assert(hashmap);
    assert(apply);

    /* 记录遍历哈希表之前的时间戳 */
    uint64_t timestap = hashmap->timestap;
    
    for (int64_t i = 0; i < hashmap->bucket_num; i++)
	for (struct node *p = hashmap->buckets[i]; p; p = p->next) {
	    apply(p->key, &p->value, arg);
	    /* 遍历哈希表的过程中,不能有任何修改哈希表的操作 */
	    assert(timestap == hashmap->timestap);
	}
}

void **hashmap_to_array(T hashmap, void *end)
{
    assert(hashmap);

    void **array = MALLOC(2*hashmap_volume(hashmap) + 1, void *);
    int64_t j = 0;
    
    for (int64_t i = 0; i < hashmap->bucket_num; i++)
	for (struct node *p = hashmap->buckets[i]; p; p = p->next) {
	    array[j++] = (void *) p->key;
	    array[j++] = p->value;
	}

    array[j] = end;
    
    return array;
}

void hashmap_free(T *hashmap_p)
{
    assert(hashmap_p && *hashmap_p);

    T hashmap = *hashmap_p;
    
    if (hashmap_volume(hashmap) > 0)
	for (int64_t i = 0; i < hashmap->bucket_num; i++)
	    for (struct node *p = hashmap->buckets[i], *next; p; p = next) {
		next = p->next;
		FREE(p);
	    }

    FREE(*hashmap_p);
}

/* 根据hint创建一个新表,将hashmap中所有元素及函数拷贝到新表中,返回新表,原
 * hashmap不变.  (集合实现需要,不导出). */
T hashmap_copy(T hashmap, int32_t hint)
{
    assert(hashmap);

    T newtab = hashmap_new(hint, hashmap->cmp_key, hashmap->hash);
    
    for (int64_t i = 0; i < hashmap->bucket_num; i++)
	for (struct node *p = hashmap->buckets[i]; p; p = p->next) {
	    uint64_t j = hashmap->hash(p->key) % newtab->bucket_num;
	    struct node *q; NEW(q);
	    q->key = p->key; q->value = p->value;
	    q->next = newtab->buckets[j];
	    newtab->buckets[j] = q;
	}

    newtab->node_num = hashmap_volume(hashmap);
    
    return newtab;
}

/* 返回hashmap的cmp_key函数.(集合实现需要,不导出) */
int (*hashmap_get_cmp_key(T hashmap)) (void const *, void const *)  
{
    return hashmap->cmp_key;
}

/* 返回hashmap的hash函数. (集合实现需要,不导出) */
uint64_t (*hashmap_get_hash(T hashmap)) (void const *)
{
    return hashmap->hash;
}

/* 判断s和t的cmp_key和hash是否相同. (集合实现需要,不导出) */
bool hashmap_same_funs(T s, T t)
{
    return (s->cmp_key == t->cmp_key && s->hash == t->hash);
}
