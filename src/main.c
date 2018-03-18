#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <string.h>
#include <assert.h>
//#include "mem.h"
#include <fcntl.h>
#include "common.h"
#include "bits.h"
#include "adt.h"
#include "makedata.h"
#include "strlist.h"
#include "makedata.h"
#include <limits.h>
#include <pthread.h>
#include <unistd.h>
#include "stdhs.h"

#include "arith.c"
#include <stdint.h>
#include <ctype.h>
#include "fileio.h"
#include "filedir.h"
#include <dirent.h>
#include "array.h"
#include "seq.h"
#include "mem.h"
#include "hashmap.h"
#include "atom.h"
#include "tools.h"
#include "set.h"

#undef malloc

void bitmap_test(void);

int div16(int x)
{
    int bias = x >> 31 & 0xF;
    return (x + bias) >> 4;

}

struct b {
    int value;
    int key;
};


struct a {
    int len;
    struct b array[0];
};


void test()
{
    int local;

    printf("%p\n", &local);
}

int linenum;


/* void double_word(char *file_name, FILE *fp) */
/* { */
/*     char prev[128], word[128]; */
     
/*     linenum = 1; */
/*     prev[0] = '\0'; */

/*     while (get_word(fp, word, sizeof(word))) { */
/* 	if (isalpha(word[0]) && strcmp(word, prev) == 0) { */
/* 	    if (file_name != NULL) */
/* 		printf("%s:", file_name); */
/* 	    printf("%d: %s\n", linenum, word); */
/* 	} */
/* 	strcpy(prev, word); */

/*     } */
/* } */

void print(void **xp, void *arg)
{
    printf("%s\n", *xp);
}

int32_t compare_int(void const *x, void const *y)
{
    return  *((int32_t *) x) - *((int32_t *) y);
}

void print_ch(void **x, void *arg)
{
    printf("%d\n", **((int32_t **)x));
}

int filter_dna(int c)
{
    return (c == 'A' || c == 'C' || c == 'T' || c == 'G');
}


void write_file(int fdi, int fdo)
{
    char buf[4096];
    int n;
  
    while ((n = read(fdi, buf, 4096)) > 0)
	if (write(fdo, buf, n) != n)
	    printf("error!\n");

    if (n < 0)
	printf("error\n");
}

#define swap(a, b)				\
    {int temp = (a); (a) = (b); (b) = temp;}



/* void print_array(int array[], int num) */
/* { */
/*     for (int i = 0; i < num; i++) */
/* 	printf(" %d", array[i]); */

/*     putchar('\n'); */
/* } */

struct point
{
    int x;
    int y;
} a[5] = {{3, 6}, {1, 94}, {7, 2}, {7, 5}, {1, 1}};

int cmp_point(void *p1, void *p2)
{
    int y1 = ((struct point *) p1)->y;
    int y2 = ((struct point *) p2)->y;

    if (y1 < y2)
	return -1;
    else if (y1 == y2)
	return 0;
    else
	return 1;
}

void print_point(struct point *points, int num)
{
    for (int i = 0; i < num; i++) {
	printf("point %d\n", i+1);
	printf("x = %d, y = %d\n", points[i].x, points[i].y);
    }
}

int cmp_int(void *n1, void *n2)
{
    int i1 = *((int *) n1);
    int i2 = *((int *) n2);

    if (i1 < i2)
	return -1;
    else if (i1 == i2)
	return 0;
    else
	return 1;
}


void *worker(void *arg);

unsigned counter;

uint64_t str_hash(void const *str)
{
    uint64_t value = 0;
    char *p = (char *) str;

    while (*p)
	value += *p++;

    return value;
}

void free_key(void *const key)
{
    
}

void add(void const *key, void **value, void *arg)
{
    int *value_p = *value;

    *value_p += *(int *) arg;
}

void print_array(void **array)
{
    for (int i = 0; array[i]; i += 2)
	printf("word: %s, count: %d\n", (char *) array[i], *((int *) array[i+1]));
}

int main(int argc, char **argv)
{
    Strlist_T strlist = strlist_new(argv[1]);
    print_strlist(strlist);
    

    /* char buf[128]; */
    
    /* getword(fp, buf, 128, first, rest); */

    /* puts(buf); */

    /* char *s1 = "aaa", *s2 = "bbbb", *s3 = "xxxxxx"; */

    /* char const *atom1 = atom_string(s1); */
    /* char const *atom2 = atom_string(s2); */
    /* char const *atom3 = atom_string(s3); */

    /* printf("atom1 len: %d\n", atom_len(atom3)); */
    /* printf("%d\n", atom_remove(atom1)); */
    /* printf("%d\n", atom_remove(atom2)); */
    /* printf("%d\n", atom_remove(atom3)); */
    /* atom_reset(); */
    
    /* FILE *elements_fp = fopen("elemets", "w+"); */
    /* FILE *set_fp = fopen("set", "w+"); */
    
    /* Set_T set1 = set_new(10, NULL, NULL); */
    /* //Set_T set2 = set_new(20, NULL, NULL); */
    
    /* int set_size; */
    
    /* set_put(set1, "aaa"); */
    /* set_put(set1, "bbb"); */
    /* set_put(set1, "ccc"); */
    /* set_put(set1, "ddd"); */

    /* fwrite(set1, set_size = sizeof(set1), 1, set_fp); rewind(set_fp); */
    /* FREE(set1); */
    
    /* set_put(set2, "aaa"); */
    /* set_put(set2, "bbb"); */
    /* set_put(set2, "eee"); */
    /* set_put(set2, "fff"); */

    


    /* Set_T inter_set = set_inter(set1, set2); */
    /* Set_T union_set = set_union(set1, set2); */
    /* Set_T minus_set = set_minus(set1, set2); */
    /* Set_T diff_set = set_diff(set1, set2); */

    /* if (set_belong(set1, "aaa")) */
    /* 	printf("belong!\n"); */

    /* char *s = set_remove(set1, "aaa"); */
    
    /* printf("%s removed!", s); */
    
    /* if (set_belong(set1, "aaa")) */
    /* 	printf("belong!\n"); */
    /* else */
    /* 	printf("not belong!\n"); */

    

    /* puts("union set:"); */
    /* char **array = set_to_array(union_set, NULL); */
    /* int32_t n = set_volume(union_set); */
    /* for (int32_t i = 0; i < n; i++) */
    /* 	puts(array[i]); */

    /* putchar('\n'); */
    

    /* puts("inter set:"); */
    /* array = set_to_array(inter_set, NULL); */
    /* n = set_volume(inter_set); */
    /* for (int32_t i = 0; i < n; i++) */
    /* 	puts(array[i]); */

    /* putchar('\n'); */

    /* puts("minus set:"); */
    /* array = set_to_array(minus_set, NULL); */
    /* n = set_volume(minus_set); */
    /* for (int32_t i = 0; i < n; i++) */
    /* 	puts(array[i]); */
    /* putchar('\n'); */

    
    /* puts("diff set:"); */
    /* array = set_to_array(diff_set, NULL); */
    /* n = set_volume(diff_set); */
    /* for (int32_t i = 0; i < n; i++) */
    /* 	puts(array[i]); */


    

    /* if (argc == 1) */
    /* 	word_frequence(NULL, stdin, NULL, NULL); */
    /* else { */
    /* 	word_frequence(argv[1], efopen(argv[1], "r"), NULL, NULL); */
    /* 	word_frequence(argv[2], efopen(argv[2], "r"), NULL, NULL); */
    /* } */
    
    
    /* Hashtab_T hashtab = hashtab_new(10, strcmp, str_hash); */
    
    /* char *key1 = "aaa"; */
    /* int value1 = 1; */
    /* int value3 = 3; */

    /* char *key2 = "bbb"; */
    /* int value2 = 2; */
    /* int value4 = 4; */

    /* hashtab_put(hashtab, key1, &value1); */
    /* hashtab_put(hashtab, key2, &value2); */

    /* int *v1 = hashtab_get(hashtab, key1); */
    /* int *v2 = hashtab_get(hashtab, key2); */

    /* printf("value1: %d\n", *v1); */
    /* printf("value2: %d\n", *v2); */
    
    /* hashtab_put(hashtab, key1, &value3); */
    /* hashtab_put(hashtab, key2, &value4); */

    /* v1 = hashtab_get(hashtab, key1); */
    /* v2 = hashtab_get(hashtab, key2); */

    /* printf("value1: %d\n", *v1); */
    /* printf("value2: %d\n", *v2); */
    
    /* int add_value = 3; */
    
    /* hashtab_traverse(hashtab, add, &add_value); */
    

    /* v1 = hashtab_get(hashtab, key1); */
    /* v2 = hashtab_get(hashtab, key2); */

    /* printf("value1: %d\n", *v1); */
    /* printf("value2: %d\n", *v2); */
    
    /* int * v = hashtab_remove(hashtab, key2, free_key); */
    /* printf("removed v: %d\n", *v); */

    /* printf("key num: %ld\n", hashtab_volume(hashtab)); */
    
    /* void ** array = hashtab_to_array(hashtab, NULL); */
    /* print_array(array); */
    
    /* hashtab_free(&hashtab); */
    
    //printf("%ld\n", hashtab_volume(hashtab));



    /// int fdi = open("/home/pz/data/DNA/patterns/10w_94170", O_RDONLY);
    //int fdo = open("out", O_WRONLY);

    //set_fl(fdo, O_SYNC);

    //show_stat(argv[1]);

    /* DIR *dir = opendir("foo"); */

    /* struct dirent *d; */

    /* while (d = readdir(dir)) */
    /*   printf("%u: %s\n", d->d_ino, d->d_name); */
  
    //Seq_T strseq = seq_new(0);
    /* Seq_T strseq = seq_new(0); */
  
    /* for (int i = 0; i < 20; i++) { */
    /*   char *str = MALLOC(100, char); */
    /*   sprintf(str, "%d", i); */
    /*   seq_push_back(strseq, str); */
    /* } */

    /* puts("Seq:"); */
    /* for (int i = 0; i < seq_len(strseq); i++) */
    /*   printf("%s\n", seq_get(strseq, i)); */

    /* puts("Seq:"); */
    /* while (seq_len(strseq)) */
    /*   printf("%s\n",seq_pop_back(strseq)); */

    /* Stack_T stk = stack_new(); */

    /* stack_push(stk, "aaa"); */
    /* stack_push(stk, "bbb"); */
    /* printf("Stack size: %ld\n", stack_size(stk)); */


    /* printf("Pop: %s\n", stack_pop(stk)); */
    /* printf("Pop: %s\n", stack_pop(stk)); */
    //stack_pop(stk);
    /* Array_T arr = array_new(10, int); */
    /* array_free(&arr); */

    /* int *p = malloc(sizeof(int)); */
    /* *p = 0; */

    /* printf("(%d) address: %p\n", getpid(), p); */

    /* while (1) { */
    /*   sleep(1); */
    /*   *p += 1; */
    /*   printf("(%d) %p: %d\n", getpid(), p, *p); */
    /* } */
  
    /* int rc; */

    /* show_stat("test"); */


    /* exit(1); */

    /* if ((rc = fork()) < 0) { */
    /*   printf("fork error!\n"); */
    /*   exit(1); */
    /* } else if (rc > 0) { */
    /*   wait(NULL); */
    /*   for (int i = 0; i < 5; i++) */
    /*     printf("b"); */
    /*   /\* printf("uid: %d\n", getuid()); *\/ */
    /*   /\* printf("euid: %d\n", geteuid()); *\/ */
    /*   /\* printf("gid: %d\n", getgid()); *\/ */
    /*   /\* printf("gid: %d\n", getegid()); *\/ */
    /* } else { */
    /*   close(STDOUT_FILENO); */
    /*   open("output", O_RDWR | O_TRUNC); */
    
    /*   for (int i = 0; i < 5; i++) */
    /*     printf("a"); */
    /*   /\* printf("gid: %d\n", getgid()); *\/ */
    /*   /\* printf("gid: %d\n", getegid()); *\/ */
    /* } */


    /* struct timespec times[2]; */
    /* struct stat statbuf; */

    //  show_stat(argv[1]);

    /* mkdir("test_dir", S_IRWXU | S_IRWXG | S_IRWXO); */
    /* chown() */
    //rmdir("test_dir");
    /* stat(argv[1], &statbuf); */

    /* times[0] = statbuf.st_atim; */
    /* times[1] = statbuf.st_mtim; */

    /* int fd = open(argv[1], O_RDWR | O_TRUNC); */

    /* if (futimens(fd, times) == -1) */
    /*   printf("futimens error!\n"); */
}

void *worker(void *arg)
{
    unsigned n = (unsigned) arg;

    while (n--) 
	counter++;

    return NULL;
}
