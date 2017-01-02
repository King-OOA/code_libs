#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#include "mem.h"
#include "common.h"
#include "makedata.h"
#include "patset.h"

static char printed_ch[] = {'a','b','c','d','e','f','g','h','i',
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

static char DNA_ch[] = {'A','C','T','G'};

/* 从长为ch_num的ch_array中选择字符，产生长为text_len的文本 */
void make_rand_text(const char *filename, int64_t text_len, Char_T *ch_array,  int32_t ch_num)
{
     assert(text_len > 0);
     assert(ch_array);
     assert(ch_num > 0);
     
     FILE *text_fp = efopen(filename, "w");

     srand(time(NULL));
     
     while (text_len--)
       putc(ch_array[rand_range(0, ch_num-1)], text_fp);
	
     efclose(text_fp);
}

  
#define ACMa 16807
#define ACMm 2147483647
#define ACMq 127773         
#define ACMr 2836
#define hi (Seed / ACMq)
#define lo (Seed % ACMq)
static int fst = 1;
static int Seed;

static int aleat(int top)
{
    long test;
    struct timeval t;
     
    if (fst) {
        gettimeofday(&t, NULL);
        Seed = t.tv_sec * t.tv_usec;
        fst = 0;
    }
     
    Seed = ((test = ACMa * lo - ACMr * hi) > 0) ? test : test + ACMm;
    return ((double) Seed) * top / ACMm;
}

/* 从文件file中, 抽取pat_num个模式串, 构成模式串文件pat_file,
   串长分布:min_pat_len~max_pat_len, 禁止的字符在保存在forbid_ch中 */
void extract_pats(char const *text_file_name, char const *pats_file_name,
		  Pat_Num_T pat_num, Pat_Len_T min_pat_len, Pat_Len_T max_pat_len,
		  char const *forbid_ch)
{
    size_t text_len;
    char *text_buf= load_file(text_file_name, &text_len);

    FILE *pats_fp = efopen(pats_file_name, "w");

    while (pat_num--) {
        Pat_Len_T pat_len = rand_range(min_pat_len, max_pat_len); /* 随机产生模式串长 */
	Pat_Len_T i;
	size_t pat_pos; /* 每个模式串在文本中的起始位置 */
        /*对每一个产生的起始位置,检查该位置的pat是否包含禁止字符,如果包含,
	  则重新产生一个起始位置,直到该位置的pat不包含任何禁止字符 */
	do { 
             pat_pos = aleat(text_len - pat_len + 1);
            for (i = 0; i < pat_len; i++)
                if (strchr(forbid_ch, text_buf[pat_pos+i])) break;
        } while (i < pat_len);
	
        for (Pat_Len_T i = 0; i < pat_len; i++)
            putc(text_buf[pat_pos+i], pats_fp);
        
        putc('\n', pats_fp);      /* 模式串以行为单位 */
    }

    efclose(pats_fp);
	  
    printf("File %s successfully generated\n", pats_file_name);

    FREE(text_buf);
}

void file_filter(char const *file_name, int32_t (*filter)(int32_t ch))
{
  FILE *ifile_fp = efopen(file_name, "rb+");
  char const *ofile_name = str_cat(file_name, "_filtered");
  FILE *ofile_fp = efopen(ofile_name, "w");
  FREE(ofile_name);

  int32_t ch;
  uint64_t n = 0; /* 被过滤掉的字符数 */
  printf("\nFiltering: %s\n", file_name);

 while ((ch = getc(ifile_fp)) != EOF)
    if (filter(ch))
      putc(ch, ofile_fp);
    else
      n++;
  
 printf("Dnoe! %ld characters filtered!\n", n);
  efclose(ofile_fp);
  efclose(ifile_fp);
}

void make_pats_file(void)
{
  char forbid_ch[] = {'\n', '\b', '\0'};

  uint64_t pats_num[] = {100000, 200000, 300000, 400000,
			 500000, 600000, 700000,
			 800000, 900000, 1000000,
			 2000000, 3000000, 4000000,
			 5000000, 6000000, 7000000,
			 8000000, 9000000, 10000000,
			 20000000, 30000000};
  
  char *pats_file_suffix[] = {"10w", "20w", "30w", "40w", "50w", "60w",
			   "70w", "80w", "90w", "100w", "200w", "300w",
			   "400w", "500w", "600w", "700w", "800w", "900w",
			   "1000w", "2000w", "3000w"};

  int pats_file_num = sizeof(pats_num) / sizeof(*pats_num);

  for (int i = 0; i < pats_file_num; i++) {
    char pats_file_path[200] = "/home/pz/data/";
    extract_pats("/home/pz/data/Random/patterns/Random_100M",
		 strcat(pats_file_path, pats_file_suffix[i]),
		 pats_num[i], MIN_PAT_LEN, MAX_PAT_LEN, forbid_ch);
  }
}

/* 产生num个长为len的DNA序列, 保存在文件file_name中 */
void make_DNA(int32_t num, int32_t len, char const *file_name)
{
    FILE *fp = efopen(file_name, "w");
    char DNA[] = {'A', 'C', 'T', 'G'};
    
    while (num--) {
	for (int i = 0; i < len; i++)
	    putc(DNA[rand_range(0, 3)], fp);
	putc('\n', fp);
    }

    efclose(fp);
}

/* 用1~alphabet中的字符填充buf的前n个字符*/
/* static void fill_buffer(unsigned char *buf, int n, int alphabet) */
/* { */
/*     int i; */
/*     long test; */
/*     struct timeval t; */
     
/*     if (fst) { */
/*         gettimeofday(&t, NULL); */
/*         Seed = t.tv_sec * t.tv_usec; */
/*         fst = 0; */
/*     } */
 
/*    for (i = 0; i < n; i++) { */
/*         Seed = ((test = ACMa * lo - ACMr * hi) > 0) ? test : test + ACMm; */
/*         buf[i] = 1 + ((double)Seed) * alphabet / ACMm; */
/*     } */
/* } */

/* 生成 file_name, 大小为 n MB, 字符集为alphabet  */
/* void fill_file(char const *file_name, int n, int alphabet) */
/* { */
/*     FILE *ofile; */

/*     if ((n < 0) || (n > 4096)) { */
/*         fprintf (stderr,"Error: file length (which is given in MB) must" */
/*                  " be between 0 and 4096 (= 4 GB file)\n"); */
/*         exit(EXIT_FAILURE); */
/*     } */

/*     if ((alphabet < 1) || (alphabet > 256)) { */
/*         fprintf (stderr,"Error: alphabet size must be between 1 and 256\n"); */
/*         exit(EXIT_FAILURE); */
/*     } */

/*     if (alphabet == 256) { */
/*         fprintf (stderr,"Warning: several indexes need alphabet size < 256" */
/*                  " to work properly.\n Generating anyway.\n"); */
/*     } */

/*     if ((ofile = fopen(file_name,"w")) == NULL) { */
/*         perror(file_name); */
/*         exit(EXIT_FAILURE); */
/*     } */

/*     while (n--) { */
/*         fill_buffer(buf, 1024*1024, alphabet); /\* 先填充块儿 *\/ */
/*         if (fwrite (buf,1024*1024,1,ofile) != 1) { /\* 再将块儿写入文件 *\/ */
/*             fprintf (stderr,"Error: cannot write %s\n",file_name); */
/*             fprintf(stderr, "%s\n", strerror(errno)); */
/*             fclose(ofile); */
/*             unlink (file_name); */
/*             exit(EXIT_FAILURE); */
/*         } */
/*     } */

/*     fclose(ofile); */

/*     fprintf (stderr,"File %s successfully generated\n",file_name); */
/* } */
//将字符串forbid中的字符序列,转化为真正的转义字符序列存入*forbide中
/* static void parse_forbid(char const *forbid, char ** forbide) */
/* { */
/*      int len, i, j; */

/*      len = strlen(forbid); */

/*      *forbide = MALLOC(len + 1, char); */
     
/*      for(i = 0, j = 0; i < len; i++) { /\* 遍历forbid字符串 *\/ */
/*          if (forbid[i] != '\\') { */
/* 	       if(forbid[i] != '\n') */
/* 		    (*forbide)[j++] = forbid[i]; */
/* 	  } else { */
/* 	       i++; */
/* 	       if(i == len) { */
/* 		    /\* forbid[i-1] = '\0'; *\/ */
/* 		    (*forbide)[j] = '\0'; */
/* 		    fprintf (stderr, "Not correct forbidden string: only one \\\n"); */
/* 		    return; */
/* 	       } */
/* 	       switch (forbid[i]) { */
/* 		   case'n':  (*forbide)[j++] = '\n'; break; */
/* 		   case'\\': (*forbide)[j++] = '\\'; break; */
/* 		   case'b':  (*forbide)[j++] = '\b'; break; */
/* 		   case'e':  (*forbide)[j++] = '\e'; break; */
/* 		   case'f':  (*forbide)[j++] = '\f'; break; */
/* 		   case'r':  (*forbide)[j++] = '\r'; break; */
/* 		   case't':  (*forbide)[j++] = '\t'; break; */
/* 		   case'v':  (*forbide)[j++] = '\v'; break; */
/* 		   case'a':  (*forbide)[j++] = '\a'; break; */
/* 		   case'c':     /\* \c字符编码(0~255) *\/ */
/* 			if(i + 3 >= len) { */
/* 			     /\* forbid[i-1] = '\0'; *\/ */
/* 			     (*forbide)[j] = '\0'; */
/* 			     fprintf (stderr, "Not correct forbidden string: 3 digits after \\c\n"); */
/* 			     return; */
/* 			} */
/* 			(*forbide)[j++] = (forbid[i+1]-48)*100 + (forbid[i+2]-48)*10 + (forbid[i+3]-48); */
/* 			i += 3; */
/* 			break; */
/* 		   default: */
/* 			fprintf (stdout, "Unknown escape sequence '\\%c'in forbidden string\n", forbid[i]); */
/* 			break; */
/* 	       } */
/* 	  } */
/*      } */
/*      (*forbide)[j] = '\0'; */
/* } */



