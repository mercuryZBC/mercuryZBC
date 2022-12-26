#include<THREAD_SERVER.h>


void* Business_Accpet(void* arg){
    int serverFd = *(int*)arg;
    struct sockaddr_in ClientAddr;
    int ClientFd;
    socklen_t Addrlen;
    struct epoll_event Node;
    char Response[_BUFSIZE];
    char client_ip[_IPSIZE];
    Addrlen = sizeof(ClientAddr);
    if((ClientFd = accept(serverFd, (struct sockaddr*)&ClientAddr, &Addrlen)) == -1){
        perror("Business_Accept error, Accept Failed");
    }
    Node.events = EPOLLIN|EPOLLET|EPOLLONESHOT;
    Node.data.fd = ClientFd;
    epoll_ctl(Epfd, EPOLL_CTL_ADD, ClientFd, &Node);
    for(int i = 0; i < _EPOLL_MAX; i++){
        if(Sock_array[i] == -1){
            Sock_array[i] = ClientFd;
            break;
        }
    }
    //设置监听
    Node.events = EPOLLIN|EPOLLET|EPOLLONESHOT;
    Node.data.fd = ClientFd;
    if(epoll_ctl(Epfd, EPOLL_CTL_ADD,ClientFd, &Node) == -1){
        perror("Business_Accept, epokk_ctl() Listen Clientfd Failed");
    }
    inet_ntop(AF_INET, &ClientAddr.sin_addr.s_addr, client_ip, _IPSIZE);
    bzero(Response,sizeof(Response));
    sprintf(Response, "hi %s Welcome Linux thread Server Demo.", client_ip);
    send(ClientFd, Response, strlen(Response), MSG_NOSIGNAL);
    printf("Server Customer 0x%x, Exec Business_Accept, Client Connection success, ip %s, port %d, sockfd %d\n", 
    (unsigned int)pthread_self(), client_ip,ntohs(ClientAddr.sin_addr.s_addr), ClientFd);
    return NULL;
}