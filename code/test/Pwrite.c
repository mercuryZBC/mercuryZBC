
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

#define MSG "Can u Hear Me???"
//写端
//打开管道文件
int main(){
  int fd = open("pipe" ,O_WRONLY);
  printf("testing\n");
  write(fd ,MSG ,strlen(MSG));
  printf("PWrite %d OK!\n" ,getpid());
  close(fd);
  return 0;
}
