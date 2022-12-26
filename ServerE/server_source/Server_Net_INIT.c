#include<THREAD_SERVER.h>


int Server_Net_INIT(void){
    int sockfd;
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_SERVER_PORT);
    inet_pton(AF_INET,_SERVER_IP,&server_addr.sin_addr.s_addr);
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
        perror("Server_NET_INET,Socket Create error");
    }
    if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        perror("Server_NET_INET,Bind Failed");
    }
    if((listen(sockfd, _BACKLOG)) == -1 ){
        perror("Server_NET_INET,Listen Failed");
    } 
    return sockfd;
}