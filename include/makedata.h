#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "patset.h"



typedef char Char_T;


char printed_ch[] = {'a','b','c','d','e','f','g','h','i',
		     'j','k','l','m','n','o','p','q','r',
		     's','t','u','v','w','x','y','z',
		     'A', 'B','C','D','E','F','G','H','I','J',
		     'K','L','M','N','O','P','Q','R','S',
		     'T','U','V','W','X','Y','Z',
		     '0','1','2','3','4','5','6','7','8','9',
		     '~', '`','!','@','#','$','%','^','&','*','(',')',
		     '-','_','=','+','{','}','[',']','\\','|',
		     ';',':','\'','"',',','.','<','>','/','?',' ',
		     '\n','\t'};

char DNA_ch[] = {'A','C','T','G'};

void make_rand_text(const char *filename, int64_t text_len, Char_T *ch_array,  int32_t ch_num);
void extract_pats(char const *text_file_name, char const *pats_file_name,
		  Pat_Num_T pat_num, Pat_Len_T min_pat_len, Pat_Len_T max_pat_len,
		  char const *forbid_ch);
void file_filter(char const *file_name, int32_t (*filter)(int32_t ch));
void make_pats_file(void);
