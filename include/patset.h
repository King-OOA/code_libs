#pragma once 

#include "adt.h"
#include <stdint.h>

#define MIN_PAT_LEN 1
#define MAX_PAT_LEN 2000


#define PRINT_PAT_NUM 1 /* 是否打印串长分布信息 */
#define PRINT_PAT 0 /* 是否打印每个模式串 */

typedef uint32_t Pat_Num_T;
typedef uint8_t Pat_Len_T;

#define T Patset_T
typedef struct T *T;

struct T {
    char *pats_file_name; /* 模式集文件名 */
    List_T pat_list; /*模式串链表*/
    Pat_Num_T pat_num; /* 模式串数量 */
    Pat_Len_T max_patlen; /*最大串长*/
    Pat_Len_T min_patlen; /*最小串长*/
    uint64_t total_patlen; /*串长之和*/
    double mean_patlen; /*平均串长*/
    double patlen_sd; /*标准差*/
    Pat_Num_T patlen_num[MAX_PAT_LEN+1]; /*串长分布，记录每一个长度含有模式串的个数*/
};

T patset_new(const char *pat_file_name);
void patset_free(T *patset);
void print_patset(const T patset);

#undef T
