#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>

//读端
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
    ptr = mmap(NULL ,size ,PROT_READ|PROT_WRITE ,MAP_SHARED , fd ,0);

    while(1){
        printf("Data: id %d Name %s\n", ptr->id, ptr->name);
        sleep(1);
    }
    munmap(ptr, size);
    close(fd);
    return 0;


}