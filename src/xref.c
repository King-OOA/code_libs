#include "stdhs.h"
#include "tools.h"
#include "hashmap.h"
#include "mem.h"
#include "atom.h"
#include "common.h"
#include "set.h"

int32_t line_num;

/* 字母，下划线 */
static int32_t first(int32_t ch)
{
    if (ch == '\n')
	line_num++;
    
    return isalpha(ch) || ch == '_';
}

/* 字母，数字，下划线 */
static int32_t rest(int32_t ch)
{
    return isalpha(ch) || isdigit(ch) || ch == '_' ;
}

static int32_t cmpint(void const *x, void const *y)
{
    int32_t n1 = *(int32_t *)x;
    int32_t n2 = *(int32_t *)y;

    if (n1 > n2)
	return 1;
    else if (n1 < n2)
	return -1;
    else
	return 0;
}

static int32_t hashint(void const *n)
{
    return *(int32_t *)n;
}

void xref(const *file, FILE *fp, Hashmap_T ids)
{
    char buf[128];

    if (file == NULL) file = "";
    
    line_num = 1;
    file = atom_string(file);

    while (get_word(fp, buf, sizeof buf, first, rest)) {
	char const *id = atom_string(buf);
	
	Hashmap_T files = hashmap_get(ids, id);
	if (files == NULL) {
	    files = hashmap_new(0, NULL, NULL);
	    hashmap_put(ids, id, files);
	}

	Set_T nums = hashmap_get(files, file);
	if (nums == NULL) {
	    nums = set_new(0, intcmp, inthash);
	    hashmap_put(files, file, nums);
	}

	if (!set_belong(nums, &line_num)) {
	    int32_t *n; NEW(n);
	    n = line_num;
	    set_put(nums, n);
	}
    }
}

int main(int argc, char **argv)
{
    Hashmap_T ids = hashmap_new(0, NULL, NULL);

    for (int32_t i = 1; i < argc; i++) {
	FILE *fp = efopen(argv[i], "r");
	xref(argv[i], fp, ids);
	efclose(fp);
    }

    /* 若无文件参数， 则处理标准输入 */
    if (argc == 1)
	xref(NULL, stdin, ids);


    exit(EXIT_SUCCESS);
}
