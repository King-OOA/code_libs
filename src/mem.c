#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include "mem.h"

#if  (!CHECK) /* 产品实现 */

void *mem_alloc(int64_t nbytes, char const *file, int32_t line)
{
  assert(nbytes > 0);

  void *p = malloc(nbytes);
  
  if (p == NULL) {
    perror("mem_alloc");
    fprintf(stderr, "At %s, line %d.\n", file, line);
    exit(EXIT_FAILURE);
  }
     
  return p;
}

void *mem_calloc(int64_t count, int64_t nbytes, char const *file, int32_t line)
{
  assert(count > 0);
  assert(nbytes > 0);
  
  void *p = calloc(count, nbytes);
  
  if (p == NULL) {
    perror("mem_calloc");
    fprintf(stderr, "At %s, line %d.\n", file, line);
    exit(EXIT_FAILURE);
  }
       
  return p;
}

/* 为了和稽核实现保持接口一致,加上文件名和行参数 */
void mem_free(void *p, char const *file, int32_t line)
{
  if (p) free(p);
}

void *mem_resize(void *p, int64_t nbytes, char const *file, int32_t line)
{
  assert(p);
  assert(nbytes > 0);
     
  if ((p = realloc(p, nbytes)) == NULL) {
    perror("mem_resize");
    fprintf(stderr, "At %s, line %d.\n", file, line);
    exit(EXIT_FAILURE);
  }

  return p;
}

#else  /* 稽核实现 */

/* 内存描述符 */
typedef struct descriptor {
  struct descriptor *f_link;    /* 空闲链表指针 */
  struct descriptor *h_link;	/* 哈希链表指针 */
  const void *mem;		/* 内存块地址 */
  int64_t size;			/* 内存块大小 */
  const char *file;		/* 在哪个文件中被分配 */
  int32_t line;			/* 在哪一行被分配 */
} Descriptor_T;

#define TAB_SIZE 2048
Descriptor_T *htab[TAB_SIZE];	/* 内存描述符哈希表 */

/* 空闲链表为首尾相连的环形链表 */
static Descriptor_T free_list_head = {&free_list_head}; /* 空闲链表头节点 */
static Descriptor_T * const free_list = &free_list_head; /* 空闲链表头指针 */

#define HASH(p, t) (((uint64_t) (p) >> 3) & (sizeof (t)/sizeof (*(t)) - 1))
#define ALIGN 8 /* 最严格的对齐 */

/* 在哈希表中查找指定内存地址是否已经被分配 */
static Descriptor_T *find(void const *mem)
{
  Descriptor_T *dp;
     
  for (dp = htab[HASH(mem, htab)];
       dp && dp->mem != mem; dp = dp->h_link)
    ;

  return dp;
}

void mem_free(void *mem, const char *file, int32_t line)
{
  if (mem) {
    Descriptor_T *dp;
    /* 检查待释放内存的合法性 */
    if ((uint64_t) mem % ALIGN != 0 /* 该内存地址不满足对其要求 */
	|| (dp = find(mem)) == NULL /* 该内存先前没有被分配过 */
	|| dp->f_link) {	    /* 该内存已经被释放掉 */
      fprintf(stderr, "mem_free: Faile to free!\n At %s, line %d\n", file, line);
      exit(EXIT_FAILURE);
    }
    /* 若合法,则将该内存块(描述符)插入到空闲链表头 */
    dp->f_link = free_list->f_link;
    free_list->f_link = dp;
  }
}

void *mem_resize(void *mem, int64_t nbytes, const char *file, int32_t line)
{
  assert(mem);
  assert(nbytes > 0);
     
  Descriptor_T *dp;
  /* 检查待resize内存的合法性 */
  if ((uint64_t) mem % ALIGN != 0
      || (dp = find(mem)) == NULL || dp->f_link != NULL) {
    fprintf(stderr, "mem_resize: Invalid memory!\n At %s, line %d\n", file, line);
    exit(EXIT_FAILURE);
  }
  
  void *new_mem = mem_alloc(nbytes, file, line);
  memcpy(new_mem, mem, nbytes < dp->size ? nbytes : dp->size);
  mem_free(mem, file, line);

  return new_mem;
}

void *mem_calloc(int64_t count, int64_t nbytes, const char *file, int32_t line)
{
  assert(count > 0);
  assert(nbytes > 0);

  void *mem = mem_alloc(count * nbytes, file, line);
  memset(mem, 0, count * nbytes);
     
  return mem;
}

#define NDESCRIPTORS 512	/* 一次性分配描述符的数量 */

/* 分配新的内存描述符 */
static Descriptor_T *dalloc(void *mem, int64_t size, const char *file, int32_t line)
{
  static Descriptor_T *dp; /* 描述符池 */
  static int32_t nleft;	   /* 池中剩余的描述符数量 */

  if (nleft <= 0) { /* 若描述符池空,则一次性分配多个描述符 */
    if ((dp = malloc(NDESCRIPTORS * sizeof(*dp))) == NULL) /* 描述符本身用malloc分配,无需再用描述符记录 */
      return NULL;
    nleft = NDESCRIPTORS;
  }
     
  dp->mem = mem;
  dp->size = size;
  dp->file = file;
  dp->line = line;
  dp->h_link = dp->f_link = NULL;
  nleft--;

  return dp++;
}

#define ROUND_UP(n, m) (((n) + (m) - 1) / (m) * (m)) /* 将n向上舍入到m的整数倍 */

void *mem_alloc(int64_t nbytes, const char *file, int32_t line)
{
  assert(nbytes > 0);
     
  nbytes = ROUND_UP(nbytes, ALIGN);
  
  /* 总是从空闲链表中分配新的内存块 */
  for (Descriptor_T *dp = free_list->f_link; dp; dp = dp->f_link) {
    Descriptor_T *new_dp;
    void *new_mem;
	  
    if (dp->size > nbytes) { /* 找到第一个合适的空闲块 */
      dp->size -= nbytes;
      new_mem = (char *) dp->mem + dp->size; /* 每次从空闲块的底部开始分配 */
      if ((new_dp = dalloc(new_mem, nbytes, file, line)) == NULL) { /* 分配相应的描述符 */
	fprintf(stderr, "%s: Allocation failed!\nAt %s, line %d\n", __FUNCTION__, file, line);
	exit(EXIT_FAILURE);
      }
      /* 将新分配的块,加入到对应哈希链表的表头 */
      uint32_t h = HASH(new_mem, htab);
      new_dp->h_link = htab[h];
      htab[h] = new_dp;
      return new_mem;
    }

    if (dp == free_list) { /* 没有合适的空闲块,则调用malloc分配 */
      nbytes += ROUND_UP(4096, ALIGN); /* 额外多分配4096字节 */
      if ((new_mem = malloc(nbytes)) == NULL ||
	  (new_dp = dalloc(new_mem, nbytes, file, line)) == NULL) {
	fprintf(stderr, "%s: Allocation failed!\nAt %s, line %d\n", __FUNCTION__, file, line);
	exit(EXIT_FAILURE);
      }
      /* 新分配的内存块先插入到空闲链表头,下一轮循环时,将会被第一个取出分配 */
      new_dp->f_link = free_list->f_link;
      free_list->f_link = new_dp;
    }
  }

  assert(0); return NULL;
}

#endif 
