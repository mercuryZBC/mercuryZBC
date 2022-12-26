#include<THREAD_SERVER.h>


int Server_start(void){
    int sockfd;
    pool_t* ptr = NULL;
    sockfd = Server_Net_INIT();
    Epoll_Create(sockfd);
    pthread_mutex_init(&array_lock, NULL);
    pthread_mutex_init(&accept_lock, NULL);
    for(int i = 0; i <_EPOLL_MAX; i++){
        Sock_array[i] = -1;
    }
    ptr = Server_Thread_Create(200, 10, 500);
    Epoll_Listen(sockfd, ptr);
    return 0;

}