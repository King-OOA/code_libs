#include "tools.h"
#include "hashmap.h"
#include "mem.h"
#include "atom.h"

/* 单词必须以字母打头 */
/* static int32_t wordfirst(char ch) */
/* { */
/*     return isalpha(ch); */
/* } */

/* /\* 剩余部分可以是字母或下划线 *\/ */
/* static int32_t wordrest(char ch) */
/* { */
/*     return isalpha(ch) || ch == '_'; */
/* } */

/* int get_word(FILE *fp, char *buf, int32_t size, int32_t first(int ch), int32_t rest(int ch)) */
/* { */
/*     assert(fp && buf && size > 1); */

/*     if (first == NULL) first = wordfirst; */
/*     if (rest == NULL) rest = wordrest; */

/*     int32_t ch, i = 0; */
    
/*     /\* 连续读取，直到找到合法的单词首字符 *\/ */
/*     for (ch = getc(fp); ch != EOF && !first(ch); ch = getc(fp)) */
/* 	; */
    
/*     /\* 保存单词首字符 *\/ */
/*     if (ch != EOF) */
/* 	buf[i++] = ch; */
    
/*     /\* 保存单词剩余的字符, 超过buf长度的部分将被舍弃 *\/ */
/*     for (ch = getc(fp); ch != EOF && rest(ch); ch = getc(fp)) */
/* 	if (i < size - 1) */
/* 	    buf[i++] = ch; */
    
/*     buf[i] = '\0'; */

/*     if (ch != EOF) */
/* 	ungetc(ch, fp); */

/*     return i > 0; */
/* } */

/* static void free_count(void const *key, void **value_p, void *arg) */
/* { */
/*     FREE(*(value_p)); */
/* } */

/* static int32_t cmp_str(void const *x, void const *y) */
/* { */
/*     return strcmp(*(char **)x, *(char **)y); */
/* } */

/* void word_frequence(char *filename, FILE *fp, int32_t first(char ch), int32_t rest(char ch)) */
/* { */
/*     Hashmap_T hashmap = hashmap_new(1000, NULL, NULL); */
/*     char word[128]; /\* 单词长度不超过127 *\/ */
/*     char const *atom; */

/*     /\* 默认首字符为字母 *\/ */
/*     if (first == NULL) */
/* 	first = wordfirst; */

/*     /\* 默认剩余字符为字母或下划线 *\/ */
/*     if (rest == NULL) */
/* 	rest = wordrest; */
	    
/*     /\*先统计*\/ */
/*     while ((get_word(fp, word, sizeof(word), first, rest))) { */
/* 	for (int i = 0; word[i]; i++) */
/* 	    word[i] = tolower(word[i]); */
/* 	atom = atom_string(word); */
/* 	int32_t *count_p = hashmap_get(hashmap, atom); */
/* 	if (count_p) */
/* 	    (*count_p)++; */
/* 	else { /\* 不存在 *\/ */
/* 	    NEW(count_p); */
/* 	    *count_p = 1; */
/* 	    hashmap_put(hashmap, atom, count_p); */
/* 	} */
/*     } */
    
/*     /\*再输出*\/ */
/*     if (filename) */
/* 	printf("%s:\n", filename); */

/*     /\* 排序 *\/ */
/*     void **array = hashmap_to_array(hashmap, NULL); */
/*     qsort(array, hashmap_volume(hashmap), 2 * sizeof(*array), cmp_str); */
    
/*     /\* 输出 *\/ */
/*     for (int i = 0; array[i]; i += 2) */
/* 	printf("%d\t%s\n", *((int *) array[i+1]), (char *) array[i]); */
    
/*     /\* 销毁数据结构 *\/ */
/*     hashmap_traverse(hashmap, free_count, NULL); */
/*     hashmap_free(&hashmap); */
/*     FREE(array); */

/*     /\* 删除每个原子 *\/ */
/*     atom_clean(); */
/* } */

