#include<stdio.h>
#include<sys/wait.h>
#include<stdlib.h>



int main(void){
    pid_t pid;
    int i = 0;
    for(i ;i <2 ;i++){
        pid = fork();
        if(pid == 0) break;
    }
    if(pid > 0){
        printf("Parent %d R...\n" ,getpid());
        int status;
        pid_t zpid;
        while((zpid = waitpid(-1 ,&status ,WNOHANG)) != -1){
            if(zpid > 0){
                if(WIFEXITED(status)){
                    printf("Parent %d:回收成功，子进程为正常退出，退出码/返回值 %d\n" ,getpid(),WEXITSTATUS(status));
                }
                if(WIFSIGNALED(status)){
                    printf("Parent %d:回收成功，子进程为异常退出，退出码/返回值 %d\n" ,getpid(),WTERMSIG(status));
                }
            }
            else if(zpid == 0){}
        }
    }else if(pid == 0){
        if(i == 0){
            sleep(5);
            printf("Child i %d Exit..\n" ,i);
            exit(0);
        }else if(i == 1){
            while(1){
                printf("Child i %d While... \n" ,i);
                sleep(1);
            }
        }
    }else{
        perror("fork failed");
        exit(0);
    }
    return 0;

}