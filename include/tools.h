#pragma once 

#include "stdhs.h"


/* 统计文件filename中单词的频率，若文件为空，则统计标准输入,单词首字符
 * 由first判断，剩余字符由rest判断 */
void word_frequence(char *filename, FILE *fp, int32_t first(int32_t ch), int32_t rest(int32_t ch));

/* 从文件中读取单词到大小为size的buf中。单词首字符由first判断，剩余字
 * 符由rest判断 */
int get_word(FILE *fp, char *buf, int32_t size, int32_t first(int32_t ch), int32_t rest(int32_t ch));
