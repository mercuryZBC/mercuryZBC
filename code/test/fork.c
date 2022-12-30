#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(void){
  pid_t pid;
  pid = fork();
  if(pid > 0){
    printf("Parent Runing...\n");//Parent
  }
  else if(pid == 0){
    printf("child Runing...\n");//child
    exit(0);
  }
  else{
    perror("Fork Call Failed");
  }
  printf("Runing..\n");
  while(1)
    sleep(1);
  return 0;


}
