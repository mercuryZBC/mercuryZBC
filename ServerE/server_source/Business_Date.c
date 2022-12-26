#include<THREAD_SERVER.h>


void* Business_Date(void* arg){
    char Buffer[_BUFSIZE];
    char Result[_BUFSIZE*2];
    int Recvlen;
    int ClientFd = *(int*)arg;
    struct epoll_event Node;
    bzero(Buffer, sizeof(Buffer));
    while((Recvlen = recv(ClientFd, Buffer, sizeof(Buffer), MSG_DONTWAIT)) > 0){
        // Forward to
        sprintf(Result, "unkonw Say: %s\n", Buffer);
        pthread_mutex_lock(&array_lock);
        for(int i = 0; i < _EPOLL_MAX; i++){
            if(Sock_array[i] != -1){
                send(Sock_array[i],Result, strlen(Result), MSG_NOSIGNAL);
            }
        }
        pthread_mutex_unlock(&array_lock);
        Node.events = EPOLLIN|EPOLLET|EPOLLONESHOT;
        epoll_ctl(Epfd, EPOLL_CTL_ADD, ClientFd, &Node);
    }
    if(Recvlen == 0){
        for(int i = 0; i < _EPOLL_MAX; i++){
            if(ClientFd == Sock_array[i]){
                Sock_array[i] = -1;
                break;
            }
        }
        epoll_ctl(Epfd, EPOLL_CTL_DEL, ClientFd, NULL);
        close(ClientFd);
        printf("Client its Done, Close Sockfd, Delete Events.\n");
    }
    return NULL;
}