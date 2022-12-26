#include<THREAD_SERVER.h>


int Epoll_Listen(int sockfd, pool_t* ptr){
    struct epoll_event ready_arr[_EPOLL_MAX];
    task_t bs;
    int readycode;
    int flag;
    printf("Server Master thread 0x%x, Listen Socket Events..\n",
    (unsigned int)pthread_self());
    while(ptr->thread_shutdown){
        if((readycode = epoll_wait(Epfd, ready_arr, _EPOLL_MAX, -1)) == -1){
           perror("Epoll_listen Error, Call Epoll_wait Failed"); 
        }
        while(readycode){
            if(ready_arr[flag].data.fd == sockfd){
                bs.Business = Business_Accept;
                bs.Business_arg = (void*)&sockfd;
                Producer_Add_Task(ptr, bs);
                printf("Server producer 0x%x Accept Business Success\n",(unsigned int)pthread_self());
            }else{
                bs.Business = Business_Date;
                bs.Business_arg = (void*)&(ready_arr[flag].data.fd);
                Producer_Add_Task(ptr, bs);
                printf("Server producer 0x%x Data Business Success\n",(unsigned int)pthread_self());
            }
            ++flag;
            --readycode;
        }
        return 0;
    }
    return 0;
}