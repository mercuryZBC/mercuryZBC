#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

#define msg "hi can you hear me"

int main(void){
    pid_t pid;
    int fds[2];
    pipe(fds);//创建管道
    pid = fork();
    if(pid > 0){
      close(fds[0]);
      write(fds[1] ,msg ,strlen(msg));
      printf("Parent %d Send msg success.\n" ,getpid());
      close(fds[1]);
      wait(NULL);
    }else if(pid == 0){
      close(fds[1]);
      char buffer[1024];
      bzero(buffer ,sizeof(buffer));
      read(fds[0] ,buffer ,sizeof(buffer));
      printf("Child %d read msg: %s \n" ,getpid() ,buffer);
      close(fds[0]);
      exit(0);
    }else{
      perror("fork call failed");
      exit(0);
    }
    
    

}
