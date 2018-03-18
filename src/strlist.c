#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include <math.h>
#include "adt.h"
#include "strlist.h"
#include "mem.h"
#include "common.h"


#define T Strlist_T

static void get_sd(void **xp, void *sd_mean)
{
    ((double *)sd_mean)[0] += pow(strlen((char *)(*xp)) - ((double*)sd_mean)[1], 2);
}

static double cal_sd(T strlist) /*计算标准差*/
{
    /* two_value[0]为sd */
    double sd_mean[2] = {0, strlist->avg_strlen}; 
    list_apply(strlist->list, get_sd, sd_mean);

    return sqrt(sd_mean[0] / strlist->str_num);
}

/* 创建字符串链表 */
T strlist_new(const char *file)
{
    FILE *fp = efopen(file, "r");; /*模式串文件*/

    T strlist;

    NEW0(strlist);
    strlist->file = strdup(file);
    strlist->list = list_new(NULL);
    strlist->min_strlen = MAX_STR_LEN;
    
    uint64_t line_num = 0;
    char buf[MAX_STR_LEN+1]; /*模式串缓存，包括换行符*/
    
    while (fgets(buf, sizeof(buf), fp)) {
      line_num++;
      char *line_break = strchr(buf, '\n'); /*换行符指针*/
        if (line_break) *line_break = '\0';
	
	Str_Len_T len = strlen(buf);
        if (len) { /*非空行*/
	    /* 插入链表末尾 */
	    list_push_back(strlist->list, strdup(buf));
	    
	    if (len < strlist->min_strlen)
		strlist->min_strlen = len;
	    else if (len > strlist->max_strlen)
		strlist->max_strlen = len;

            strlist->total_strlen += len;
            strlist->strlen_num[len]++;
        } 
    }
    
    //printf("Total line: %ld\n", line_num);
    strlist->str_num = list_len(strlist->list);
     /*计算平均串长*/
    strlist->avg_strlen = (double) strlist->total_strlen / strlist->str_num;
    /* 计算串长标准差 */
    strlist->strlen_sd = cal_sd(strlist);
    
    efclose(fp);
    
    return strlist;
}

static void free_str(void **xp, void *arg)
{
    FREE(*xp);
}

/*销毁字符串链表*/
void strlist_free(T *strlist_p) 
{
    /* 释放模式集文件名 */
    FREE((*strlist_p)->file);
    /* 释放链表节点连着的模式串 */
    list_apply((*strlist_p)->list, free_str, NULL);
    /* 释放模式集链表 */
    list_free(&(*strlist_p)->list);
    /* 释放模式集 */
    FREE(*strlist_p);
}

static void print_str(void **xp, void *arg)
{
    printf("%s\n", (char *)(*xp));
}

/*打印字符串链表信息*/
void print_strlist(const T strlist) 
{
    assert(strlist);
    
    /* 打印模式集基本信息 */
    printf("file: %s\n"
    	   "str num: %d\n"
    	   "min len: %d\n"
    	   "max len: %d\n"
    	   "total len: %ld\n"
    	   "avg len: %.2f\n"
    	   "len sd: %.2f\n",
    	   strlist->file,
    	   strlist->str_num,
    	   strlist->min_strlen,
    	   strlist->max_strlen,
    	   strlist->total_strlen,
    	   strlist->avg_strlen,
    	   strlist->strlen_sd);
    
    /* 打印串长分布 */
    if (1) {
    	printf("length    number:\n");
    	for (Str_Len_T len = 1; len <= strlist->max_strlen; len++) {
    	    Str_Num_T num = strlist->strlen_num[len];
    	    if (num) printf(" %3d  %5d %7.2f%%\n",
    		       len, num, ((double) num / strlist->str_num) * 100);
    	}
    }
    
    /* 打印每个模式串 */
    if (0)
      list_apply(strlist->list, print_str, NULL);
}
