#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "adt.h"
#include "patset.h"
#include "mem.h"
#include "common.h"


#define T Patset_T


static void free_pat(void **xp, void *arg)
{
    FREE(*xp);
}

static void print_pat(void **xp, void *arg)
{
    printf("%s\n", (char *)(*xp));
}

static void get_sd(void **xp, void *sd_mean)
{
    ((double *)sd_mean)[0] += pow(strlen((char *)(*xp)) - ((double*)sd_mean)[1], 2);
}

static double cal_sd(T patset) /*计算标准差*/
{
    double sd_mean[2] = {0, patset->mean_patlen}; /* two_value[0]为sd */
     
    list_traverse(patset->pat_list, get_sd, sd_mean);

    return sqrt(sd_mean[0] / patset->pat_num);
}

/* 创建模式集 */
T patset_new(const char *pats_file_name)
{
    FILE *pats_fp; /*模式串文件*/
    pats_fp = efopen(pats_file_name, "rb");

    T patset;

    NEW(patset);
    patset->pats_file_name = strdup(pats_file_name);
    patset->pat_list = list_new(NULL);
    patset->min_patlen = MAX_PAT_LEN;
    patset->pat_num = 0;
    patset->max_patlen = 0;
    patset->mean_patlen = 0;
    patset->total_patlen = 0;
    memset(patset->patlen_num, 0, sizeof(patset->patlen_num));
    
    uint64_t line_num = 0;

    char pat[MAX_PAT_LEN+1]; /*模式串缓存，包括换行符*/
    while (fgets(pat, sizeof(pat), pats_fp)) {
      line_num++;
      char *line_break = strchr(pat, '\n'); /*换行符指针*/
        if (line_break) *line_break = '\0';
	
	Pat_Len_T patlen = strlen(pat);
        if (patlen) { /*非空行*/
	    list_push_back(patset->pat_list, strdup(pat));
	    
	    if (patlen < patset->min_patlen)
		patset->min_patlen = patlen;
	    else if (patlen > patset->max_patlen)
		patset->max_patlen = patlen;

            patset->total_patlen += patlen;
            patset->patlen_num[patlen]++;
        } 
    }
    
    //printf("Total line: %ld\n", line_num);
    patset->pat_num = list_size(patset->pat_list);
     /*计算模式串平均长度*/
    patset->mean_patlen = (double) patset->total_patlen / patset->pat_num;
    /* 计算串长标准差 */
    patset->patlen_sd = cal_sd(patset);
    
    efclose(pats_fp);
    
    return patset;
}

/*销毁模式集*/
void patset_free(T *patset_p) 
{
    /* 释放模式集文件名 */
    FREE((*patset_p)->pats_file_name);
    /* 释放链表节点连着的模式串 */
    list_traverse((*patset_p)->pat_list, free_pat, NULL);
    /* 释放模式集链表 */
    list_free(&(*patset_p)->pat_list);
    /* 释放模式集 */
    FREE(*patset_p);
}

/*打印模式集信息*/
void print_patset(const T patset) 
{
    /* 打印模式集基本信息 */
    printf("file: %s\n"
    	   "pat num: %ld\n"
    	   "min len: %d\n"
    	   "max len: %d\n"
    	   "total len: %ld\n"
    	   "mean len: %.2f\n"
    	   "len sd: %.2f\n",
    	   patset->pats_file_name,
    	   patset->pat_num,
    	   patset->min_patlen,
    	   patset->max_patlen,
    	   patset->total_patlen,
    	   patset->mean_patlen,
    	   patset->patlen_sd);
    
    /* 打印串长分布 */
    if (PRINT_PAT_NUM) {
    	printf("\nPattern length number:\n");
    	for (Pat_Len_T patlen = 1; patlen <= patset->max_patlen; patlen++) {
    	    Pat_Num_T num = patset->patlen_num[patlen];
    	    if (num) printf("len|num:  %2d | %5ld %7.2f%%\n",
    		       patlen, num, ((double) num / patset->pat_num) * 100);
    	}
    }
    
    /* 打印每个模式串 */
    if (PRINT_PAT)
      list_traverse(patset->pat_list, print_pat, NULL);
}
