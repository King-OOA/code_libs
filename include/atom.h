#pragma once

#include <stdint.h>

#define DUP_STR 1 /*  是否复制保存原子字符串  */

typedef int32_t atom_len_t;

/* 为长为len的字节序列str,新建原子(以'\0'结尾),原子为指向该序列的指针. */
char const *atom_new(char const *str, atom_len_t len);

/* 为字符串str(以'\0'结尾的)新建原子, 若已经存在则直接返回 */
char const *atom_string(char const *str);

/* 为整数n新建原子, 整数以字符串形式存储为原子 */
char const *atom_int(int64_t n);

/* 若atom是已存在原子，返回其长度；否则，返回负值*/
int atom_len(char const *atom);

/* 从原子表中删除原子atom, 若删除成功, 返回1; 若atom不在表中,则不做任何修改, 返回0*/
int atom_remove(char const *atom);

/* 删除原子表中的所有原子, 保留表结构 */
void atom_clean(void);

