#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>


int main(void){
pid_t pid;
pid =fork();
if(pid > 0){
    printf("parent pid %d\n" ,getpid());
    while(1){
        sleep(1);
    }

}
else if(pid == 0){
    printf("child pid %d\n" ,getpid());
    sleep(10);
    execlp("firefox" ,"firefox" ,"www.baidu.com" ,NULL);

}
else{
    perror("fork call failed");
}

return 0;


}