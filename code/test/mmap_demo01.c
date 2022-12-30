#include<stdio.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/fcntl.h>


//MMAP DEMO 01


int main(void){

  int fd = open("mmfile" ,O_RDWR);
  int fsize;
  int * ptr =NULL;
  fsize = lseek(fd ,0 ,SEEK_END);
  if((ptr = mmap(NULL ,fsize ,PROT_READ|PROT_WRITE,MAP_SHARED ,fd ,0)) == MAP_FAILED){
    perror("mmap Failed");
    exit(0);
  }
  close(fd);
  ptr[0] = 0x34333231;//修改映射内存前四个字节，改为1234
  printf("Change Success, its Done..\n");
  munmap(ptr ,fsize);
  return 0;

}
