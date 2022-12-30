#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

#define MSG "Can u Hear Me???"

int main(){
  char buffer[1024];
  bzero(buffer ,sizeof(buffer));
  int fd = open("pipe" ,O_RDONLY);
  read(fd ,buffer ,sizeof(buffer));
  printf("PRead %d Read Data %s\n" ,getpid() ,buffer);
  close(fd);
  return 0;
}
