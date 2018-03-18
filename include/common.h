#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define  container_of(ptr, type, member) ({                      \
                      const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
                       (type *)( (char *)__mptr - offsetof(type,member) );})

#define ARRAY_LEN(a) ((sizeof (a)) / (sizeof (*(a))))


/* 带错误处理的fopen和fclose */
FILE *efopen(char const *file_path, char const *mode);
void efclose(FILE *fp);

char *str_cat(const char *, const char *);
char *str_dup(char const *str);
int rand_range(int low, int high);

int get_file_alphabet(char const *filename);
size_t  get_file_size(FILE *fp);
void cut_file(char const *iname, int n);
int alphabet_compact(unsigned char *text, int text_len);
char *load_file(const char *file_name, size_t *file_size_p);
int getword(FILE *fp, char *buf, int32_t size, int32_t first(char ch), int32_t rest(char ch));

