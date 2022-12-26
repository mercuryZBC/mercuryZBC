#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<arpa/inet.h>
#include<MySocket.h>
#include<errno.h>
#include<fcntl.h>
#define SERVER_IP "42.193.104.238"
#define SERVER_IPM "192.168.174.135"
#define SERVER_PORT 1234
int main(){
	char Send_mag[1024];
	char rec_mag[1024];
	int client_fd;

	client_fd = SOCKET(AF_INET, SOCK_STREAM, 0 );


	struct sockaddr_in ServerAddr;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(SERVER_PORT);
	ServerAddr.sin_addr.s_addr = inet_addr(SERVER_IPM);

	CONNECT(client_fd, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr));

    if(recv(client_fd, rec_mag, sizeof(rec_mag),0) > 0){
		printf("%s", rec_mag);
	}

	bzero(rec_mag, sizeof(rec_mag));
	bzero(Send_mag, sizeof(Send_mag));
	
	//读取读取标准输入
	int flags;
	int len;
	flags = fcntl(STDIN_FILENO,F_GETFD,0);//STDIN默认
	flags|= O_NONBLOCK;
	fcntl(STDIN_FILENO,F_SETFL,flags);//设置非阻塞
again:
	while((len = read(STDIN_FILENO,Send_mag,sizeof(Send_mag))) > 0){
	send(client_fd, &Send_mag, strlen(Send_mag),0);
	usleep(100000);//等待服务处置
	if(recv(client_fd,&rec_mag,sizeof(rec_mag),MSG_DONTWAIT) == -1){
		if(errno = EAGAIN){
			bzero(Send_mag,sizeof(Send_mag));
			continue;
		}
	}else{
		perror("recv Call Failed");
	}
	printf("%s",rec_mag);
	bzero(rec_mag,sizeof(rec_mag));
	bzero(Send_mag,sizeof(Send_mag));
	}
	if(len == -1){
		if(errno ==  EAGAIN){
			if((recv(client_fd,rec_mag,sizeof(rec_mag),MSG_DONTWAIT)) == -1){
				if(errno == EAGAIN) {
					bzero(Send_mag, sizeof(Send_mag));
					
				}
				else perror("Recv Call Failed");
			}
			else
				printf("%s\n",rec_mag);
			bzero(rec_mag,sizeof(rec_mag));
			bzero(Send_mag,sizeof(Send_mag));
		}
		
		
	}
	goto again;
	close(client_fd);
	return 0;
}
