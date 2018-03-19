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

/* 从长为size的数组alphabet中随机选择字符，产生长为str_len的文本, 存入文件file中 */
void make_rand_str(Char_T *alphabet, int32_t size, int64_t str_len, const char *ofile)
{
    assert(ofile);
    assert(str_len > 0);
    assert(alphabet);
    assert(size > 0);
     
    FILE *fp = efopen(ofile, "w");
	     
    srand(time(NULL));
     
    while (str_len--)
	putc(alphabet[rand_range(0, size-1)], fp);
	
    efclose(fp);
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

/* 从文件ifile中, 抽取str_num个串, 存入文件ofile中, 每行一个.
   串长分布:min_len~max_len, 禁止的字符在保存在forbids中 */
void extract_strs(char const *ifile, char const *ofile,
		  Str_Num_T str_num, Str_Len_T min_len, Str_Len_T max_len,
		  char const *forbids)
{
    size_t file_len;
    char *buf = load_file(ifile, &file_len);

    FILE *ofp = efopen(ofile, "w");

    while (str_num--) {
        Str_Len_T str_len = rand_range(min_len, max_len); /* 随机产生串长 */
	Str_Len_T i;
	size_t pos; /* 在位置pos抽取串 */
        /* 对每一个产生的pos, 检查该位置的str是否包含禁止字符, 如果包含,
	  则重新产生一个pos, 直到该位置的str不包含任何禁止字符 */
	do { 
	    pos = aleat(file_len - str_len + 1);
            for (i = 0; i < str_len; i++)
                if (strchr(forbids, buf[pos+i])) break;
        } while (i < str_len);
	
        for (Str_Len_T i = 0; i < str_len; i++)
            putc(buf[pos+i], ofp);
        
        putc('\n', ofp);      /* 模式串以行为单位 */
    }

    efclose(ofp);
	  
    printf("File %s successfully generated\n", ofile);

    FREE(buf);
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
	if(filter(ch))
	    putc(ch, ofile_fp);
	else
	    n++;

    printf("Dnoe! %ld characters filtered!\n", n);
    efclose(ofile_fp);
    efclose(ifile_fp);
}

void make_strs_file(void)
{
    char forbid_ch[] = {'\n', '\b', '\0'};

    uint64_t strs_num[] = {100000, 200000, 300000, 400000,
			   500000, 600000, 700000,
			   800000, 900000, 1000000,
			   2000000, 3000000, 4000000,
			   5000000, 6000000, 7000000,
			   8000000, 9000000, 10000000,
			   20000000, 30000000};
  
    char *strs_file_suffix[] = {"10w", "20w", "30w", "40w", "50w", "60w",
				"70w", "80w", "90w", "100w", "200w", "300w",
				"400w", "500w", "600w", "700w", "800w", "900w",
				"1000w", "2000w", "3000w"};

    int strs_file_num = sizeof(strs_num) / sizeof(*strs_num);

    for (int i = 0; i < strs_file_num; i++) {
	char strs_file_strh[200] = "/home/pz/data/";
	extract_strs("/home/pz/data/Random/strterns/Random_100M",
		     strcat(strs_file_strh, strs_file_suffix[i]),
		     strs_num[i], MIN_STR_LEN, MAX_STR_LEN, forbid_ch);
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



