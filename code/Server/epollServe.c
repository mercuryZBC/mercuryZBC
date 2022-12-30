#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include "MySocket.h"
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/epoll.h>

#define EPOLL_LISTEN 1024

int main()
{

    int Serverfd;
    int Clientfd;
    socklen_t AddrLen;
    struct sockaddr_in MyAddr, ClientAddr;
    time_t tp;
    char time_buf[1024];
    int err;
    int recvlen = 0;
    char recv_msg[1024];
    char send_msg[1024];
    char Response[1024];
    char Forward[2048];
    int len;
    int epfd;
    int readycode;
    struct epoll_event ready_array[EPOLL_LISTEN];
    struct epoll_event Node;
    bzero(Response, sizeof(Response));
    bzero(Forward, sizeof(Forward));
    bzero(send_msg, sizeof(send_msg));
    bzero(&MyAddr, sizeof(MyAddr));
    MyAddr.sin_family = AF_INET;
    MyAddr.sin_port = htons(1234);
    MyAddr.sin_addr.s_addr = INADDR_ANY;
    Serverfd = SOCKET(AF_INET, SOCK_STREAM, 0);

    BIND(Serverfd, (struct sockaddr *)&MyAddr, sizeof(MyAddr));

    LISTEN(Serverfd, 128);
    Node.data.fd = Serverfd;
    Node.events = EPOLLIN;
    epfd = epoll_create(EPOLL_LISTEN);
    epoll_ctl(epfd, EPOLL_CTL_ADD, Serverfd, &Node);
    printf("The POLL server 0.0.3 Accept Start\n");

    while (1)
    {

        readycode = epoll_wait(epfd, ready_array, EPOLL_LISTEN, -1); //阻塞
        if (readycode > 0)
        {
            int i = 0;
            while (readycode)
            {

                //辨别就绪
                // server_fd

                if (ready_array[i].data.fd == Serverfd)
                {

                    AddrLen = sizeof(ClientAddr);
                    Clientfd = ACCEPT(ready_array[i].data.fd, (struct sockaddr *)&ClientAddr, &AddrLen);

                    //储存socket
                    Node.data.fd = Clientfd;
                    Node.events = EPOLLIN;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, Clientfd, &Node);

                    printf("pid %d Client_sock %d port %d connet successfully..\n", getpid(), Node.data.fd, ntohs(ClientAddr.sin_port));

                    sprintf(send_msg, "welcome the server\n");
                    send(Clientfd, &send_msg, sizeof(send_msg), 0);
                    printf("Send Response");
                }
                else
                {
                    // client_fd;
                    if ((len = recv(ready_array[i].data.fd, &Response, sizeof(Response), 0)) > 0)
                    {

                        if (strcmp(Response, "localtime\n") == 0)
                        {
                            bzero(time_buf, sizeof(time_buf));
                            tp = time(NULL);
                            ctime_r(&tp, time_buf); //计算时间
                            printf("localtime Request compete\n");
                            send(ready_array[i].data.fd, time_buf, strlen(time_buf), 0);
                        }

                        else
                        {
                            
                            printf("%s\n", Response);
                        }
                        bzero(Response, sizeof(Response));
                        bzero(Forward, sizeof(Forward));
                    }
                    if (len == -1)
                    {
                        perror("Recv Call Error");
                        exit(0);
                    }
                    else if (len == 0)
                    {
                        close(ready_array[i].data.fd);
                        Node.data.fd = ready_array[i].data.fd;
                        Node.events = EPOLLIN;
                        epoll_ctl(epfd,EPOLL_CTL_DEL,ready_array[i].data.fd,&Node);
                    }

                    
                }
                --readycode;
                    i++;
            }
        }
        else if (readycode == -1)
        {
            printf("POLL Call Failed");
            exit(0);
        }
    }
    return 0;
}
