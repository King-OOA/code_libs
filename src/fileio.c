#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "fileio.h"

void show_fl(int fd)
{
    int fl;

    if ((fl = fcntl(fd, F_GETFL, 0)) == -1)
	fprintf(stderr, "fcntl error for %d\n", fd);

    switch (fl & O_ACCMODE) {
    case O_RDONLY: printf("read only");break;
    case O_WRONLY: printf("write only");break;
    case O_RDWR: printf("read write");break;
    default: printf("unknown mode\n");exit(EXIT_FAILURE);
    }

    if (fl & O_APPEND)
	printf(", append");

    if (fl & O_NONBLOCK)
	printf(", nonblock");

    if (fl & O_SYNC)
	printf(", sync");

    putchar('\n');
}

void set_fl(int fd, int fl)
{
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) == -1)
	fprintf(stderr, "fcntl F_GETFL error for %d\n", fd);

    val |= fl;

    if ((fl= fcntl(fd, F_SETFL, val)) == -1)
	fprintf(stderr, "fcntl F_SETFL error for %d\n", fd);
}

void clr_fl(int fd, int fl)
{
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) == -1)
	fprintf(stderr, "fcntl F_GETFL error for %d\n", fd);

    val &= ~fl;

    if ((fl= fcntl(fd, F_SETFL, val)) == -1)
	fprintf(stderr, "fcntl F_SETFL error for %d\n", fd);
}
