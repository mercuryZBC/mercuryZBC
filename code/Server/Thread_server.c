#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<stdlib.h>
#include"MySocket.h"
#include<pthread.h>
#include<signal.h>
#include<sys/wait.h>
 

int sock_list[1024];

pthread_mutex_t list_lock;




typedef struct{
	int sockfd;
	char client_ip[16];
}thread_arg;



	time_t tp;
	char time_buf[1024];

void* Thread_server(void* arg){
	//客户端处理流程
	char Forward[2048];
	char Response[1024];
	int len;
	pthread_detach(pthread_self());
	thread_arg targ = *(thread_arg*)arg;
	bzero(Response, sizeof(Response));
		while((len = recv(targ.sockfd,&Response,sizeof(Response),0)) > 0)//客户端断开
		{
			
			if(strcmp(Response, "localtime\n") == 0){
				bzero(time_buf, sizeof(time_buf));
				tp = time(NULL);
				ctime_r(&tp, time_buf);//计算时间
				printf("localtime Request compete\n");
				send(targ.sockfd, time_buf, strlen(time_buf), 0);
			}
			
			else{ 
				sprintf(Forward,"(%s) Say: %s\n\n", targ.client_ip,Response);
				printf("%s\n", Forward);
				for(int i = 0; i < 1024; i++){
					if(sock_list[i] != -1){
						send(sock_list[i],Forward,strlen(Forward),0);
					}
				}
				pthread_mutex_unlock(&list_lock);
					
			}
			bzero(Response, sizeof(Response));
		}
		if(len == -1){

			close(targ.sockfd);
			pthread_exit(NULL);
		}else if(len == 0){
			printf("Client_ip (%s) Exit, thread 0x%x is Exit", targ.client_ip, (unsigned int)pthread_self());
			close(targ.sockfd);
			pthread_exit(NULL);
		}


}

int main(){


	
	int Serverfd;
	int Clientfd;
	socklen_t AddrLen;
	struct sockaddr_in MyAddr,ClientAddr;
	int err;
	int recvlen = 0;
	char recv_msg[1024];
	char send_msg[1024];

	for(int i = 0; i <1024; i++){
		sock_list[i] = -1;
	}
	pthread_mutex_lock(&list_lock);
	//int client_array[1024];//存储客户端sock数组
	//for(int i = 0; i < 1024; i++){
		//client_array[i] = -1;
	//}
	// for(int i = 0; i <1024; i++){
	// 	sock_list[i] = -1;
	// }
	// pthread_mutex_init(&list_lock(&list_lock));


	bzero(&MyAddr, sizeof(MyAddr));
	MyAddr.sin_family = AF_INET;
	MyAddr.sin_port = htons(1234);
	MyAddr.sin_addr.s_addr = INADDR_ANY;
	Serverfd = SOCKET(AF_INET, SOCK_STREAM, 0);

	BIND(Serverfd, (struct sockaddr*)&MyAddr, sizeof(MyAddr));

	
	LISTEN(Serverfd, 128);
	thread_arg targ;
	pthread_t tid;
 
	while(1){
		bzero(recv_msg, sizeof(recv_msg));
		printf("The Thread server 0.0.3 Accept Start, Master thread tid 0x%x...\n",(unsigned int)pthread_self());
		AddrLen = sizeof(ClientAddr);
		Clientfd = ACCEPT(Serverfd, (struct sockaddr*)&ClientAddr , &AddrLen);
		bzero(targ.client_ip, 16);
		inet_ntop(AF_INET, &ClientAddr.sin_addr.s_addr, targ.client_ip, 16);
		targ.sockfd = Clientfd;
		//储存socket
		pthread_mutex_lock(&list_lock);
		for(int i = 0; i < 1024; i++){
			if(sock_list[i] == -1){
				sock_list[i] = Clientfd;
				break;
			}
		}
		pthread_mutex_unlock(&list_lock);
		if((err = pthread_create(&tid, NULL, Thread_server, (void*)&targ)) < 0 ){
			printf("the Thread Server Create Thread Failed: %s\n", strerror(err));
			exit(0);
		}





			
			printf("ppid %d Client ip %s port %d connet successfully..\n", getpid(), targ.client_ip, ntohs(ClientAddr.sin_port));
			sprintf(send_msg, "hello %s welcome this server\n",targ.client_ip);
			send(Clientfd,&send_msg,sizeof(send_msg),0);
	}
		close(Serverfd);
		pthread_mutex_destroy(&list_lock);
		return 0;
}

