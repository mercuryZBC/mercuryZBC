#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>

//写端
typedef struct 
{
    int id;
    char name[20];
}shared_t;




int main(){
    int fd;
    int size = sizeof(shared_t);
    fd = open("MapFile" ,O_RDWR|O_CREAT ,0664);
    shared_t *ptr = NULL;
    ftruncate(fd ,size);//截断
    ptr = mmap(NULL ,size ,PROT_READ|PROT_WRITE ,MAP_SHARED , fd ,0);
    ptr->id = 0;
    bzero(ptr->name ,sizeof(ptr->name));

    while(1){
        ++(ptr->id);
        sprintf(ptr->name, "User_%d", ptr->id);
        sleep(1);
    }
    munmap(ptr, size);
    close(fd);
    return 0;


}