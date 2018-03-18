#pragma once 

#define CUR_OFFSET(fd) lseek((fd), 0, SEEK_CUR)


void show_fl(int fd);
void set_fl(int fd, int fl);
void clr_fl(int fd, int fl);
