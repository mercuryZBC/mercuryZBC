//创建多进程模型，1父，3子
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

int main(void){
  pid_t pid;
  int i = 1;
  for(i ;i <= 3 ;i++){
    pid = fork();
    if(pid == 0) break;
  }
  if(pid > 0){
    printf("Parent Create Success pid %d.\n" ,getpid());

  }
  else if(pid == 0){
    if(i == 1){
      printf("child (%d) swimming\n", i);

    }
    else if(i == 2){
      printf("child (%d) slip\n" ,i);
    }
    else if(i == 3){
      printf("child (%d) running\n" ,i);
    }
    while(1){
      sleep(1);
    }
  }
  else{
    perror("fork Call Failture");
    exit(0);
  }

}
