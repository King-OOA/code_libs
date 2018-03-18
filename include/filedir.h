#pragma once 
#include <sys/stat.h>

void show_stat(char const *path);
void file_type(char const *path);
/* 查看文件权限 */
void file_permissions(struct stat *stat_p);
