#include<THREAD_SERVER.h>

int Epoll_Create(int sockfd){
    struct epoll_event Node;
    if((Epfd = epoll_create(_EPOLL_MAX)) == -1){
        perror("Epoll_Create, epoll_Create Call Failed");
    }
    Node.events = EPOLLIN|EPOLLET|EPOLLONESHOT;
    Node.data.fd = sockfd;
    if((epoll_ctl(Epfd, EPOLL_CTL_ADD, sockfd, &Node)) == -1){
        perror("Epoll_Create, epoll_ctl Failed, set Serverfd");
    }
    return 0;
}