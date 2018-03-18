#include "filedir.h"
#include "stdhs.h"




void file_permissions(struct stat *stat_p)
{
  /* User permissions */
  if (stat_p->st_mode & S_IRUSR)
    printf("user readable!\n");

  if (stat_p->st_mode & S_IWUSR)
    printf("user writeable!\n");
  
  if (stat_p->st_mode & S_IXUSR)
    printf("user executable!\n\n");

  /* Group permissions */
  if (stat_p->st_mode & S_IRGRP)
    printf("group readable!\n");

  if (stat_p->st_mode & S_IWGRP)
    printf("group writeable!\n");
  
  if (stat_p->st_mode & S_IXGRP)
    printf("group executable!\n\n");

  /* Other permissions */
  if (stat_p->st_mode & S_IROTH)
    printf("others readable!\n");

  if (stat_p->st_mode & S_IWOTH)
    printf("others writeable!\n");
  
  if (stat_p->st_mode & S_IXOTH)
    printf("others executable!\n");
}

void show_stat(char const *path)
{
  struct stat buf;
  
  if (lstat(path, &buf) == -1) {
	fprintf(stderr, "lstat error: %s\n", path);
	exit(EXIT_FAILURE);
    }

  file_type(path);
  printf("i-node number: %ld\n", buf.st_ino);
  printf("device number: %ld\n", buf.st_dev);
  printf("R device number: %ld\n", buf.st_rdev);
  printf("number of links: %ld\n", buf.st_nlink);
  printf("user ID: %u\n", buf.st_uid);
  printf("group ID: %u\n", buf.st_gid);
  printf("file size: %ld\n", buf.st_size);
  printf("Acess time: %ld\n", buf.st_atime);
  printf("Modification time: %ld\n", buf.st_mtime);
  printf("Status change time: %ld\n", buf.st_ctime);
  printf("number of blocks: %ld\n", buf.st_blocks);
  printf("block size: %ld\n", buf.st_blksize);
}

void file_type(char const *path)
{
    struct stat buf;
    char const *type;

    /* 必须用lstat以识别符号链接 */
    if (lstat(path, &buf) == -1) {
	fprintf(stderr, "lstat error: %s\n", path);
	exit(EXIT_FAILURE);
    }

    if (S_ISREG(buf.st_mode))
	type = "regular";
    else if (S_ISDIR(buf.st_mode))
	type = "directory";
    else if (S_ISCHR(buf.st_mode))
	type = "character special";    
    else if (S_ISBLK(buf.st_mode))
	type = "block special";
    else if (S_ISFIFO(buf.st_mode))
	type = "FIFO";
    /* else if (S_ISSOCK(buf.st_mode))  Linux 不识别 S_ISSOCK宏*/ 
    /* 	type = "socket"; */
    else if (S_ISLNK(buf.st_mode))
	type = "symbolic link";
    else
	type = "** unkonw mode **";

    printf("%s: %s file\n", path, type);
}
