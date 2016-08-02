#pragma once

#include <stdio.h>
#include <stdlib.h>

/* #define MALLOC(num,type) (type *)mlloc((num)*sizeof(type), __FILE__, __FUNCTION__, __LINE__) */
/* #define VMALLOC(struct_type,array_type,array_size) mlloc(sizeof(struct_type) + (array_size)*sizeof(array_type), __FILE__, __FUNCTION__, __LINE__) */
/* #define CALLOC(num,type) (type *)clloc(num, sizeof(type), __FILE__, __FUNCTION__, __LINE__) */

/* void *clloc(size_t, size_t, const char *, const char *, int); */
/* void *mlloc(size_t, const char *, const char *, int); */


/* 带错误处理的fopen和fclose */
FILE *efopen(char const *file_path, char const *mode);
void efclose(FILE *fp);

char *str_cat(const char *, const char *);
int rand_range(int, int);

int get_file_alphabet(char const *filename);
size_t  get_file_size(FILE *fp);
void cut_file(char const *iname, int n);
int alphabet_compact(unsigned char *text, int text_len);
char *load_file(const char *file_name, size_t *file_size_p);
