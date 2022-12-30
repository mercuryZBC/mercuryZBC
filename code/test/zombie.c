#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>


int main(void){

    pid_t pid;
    pid = fork();
    if(pid > 0){
        printf("Parent Runing...\n");
        pid_t zpid = wait(NULL);
        printf("Parent wait success , zombie pid %d\n",zpid);
        while(1) sleep(1);
    }else if(pid == 0){
        sleep(5);
        printf("Child pid %d Exiting..\n", getpid());
        exit(0);
    }else{
        perror("fork call failed");
        exit(0);
    }
    return 0;
}