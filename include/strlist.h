/* 读取文件,将文件中的每一(非空)行作为一个字符串组成链表 */
/* author: pz
   Date: 2018.3.18 */

#pragma once 

#include "adt.h"
#include <stdint.h>

#define MIN_STR_LEN 1
#define MAX_STR_LEN 2000


typedef uint32_t Str_Num_T;
typedef uint32_t Str_Len_T;

#define T Strlist_T
typedef struct T *T;

struct T {
    char *file; /* 串文件名 */
    Str_Num_T str_num; /* 串数量 */
    Str_Len_T max_strlen; /*最大串长*/
    Str_Len_T min_strlen; /*最小串长*/
    uint64_t total_strlen; /*串长之和*/
    double avg_strlen; /*平均串长*/
    double strlen_sd; /*串长标准差*/
    Str_Num_T strlen_num[MAX_STR_LEN+1]; /*串长分布, 记录每一个长度含有模式串的个数*/
    List_T list; /*串链表*/
};

T strlist_new(const char *str_file_name);
void strlist_free(T *strlist);
void print_strlist(const T strlist);

#undef T
