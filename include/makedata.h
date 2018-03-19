#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "strlist.h"



typedef char Char_T;

void make_rand_str(Char_T *alphabet, int32_t size, int64_t str_len, const char *ofile);

void extract_strs(char const *ifile, char const *ofile,
		  Str_Num_T str_num, Str_Len_T min_len, Str_Len_T max_len,
		  char const *forbids);

void file_filter(char const *file_name, int32_t (*filter)(int32_t ch));
void make_pats_file(void);
void make_DNA(int32_t num, int32_t len, char const *file_name);
