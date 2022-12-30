#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <MySocket.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/select.h>

typedef struct
{
	int sockfd;
	char client_ip[16];
} sockip;

sockip sock_list[1024];

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

	for (int i = 0; i < 1024; i++)
	{
		sock_list[i].sockfd = -1;
	}

	bzero(&MyAddr, sizeof(MyAddr));
	MyAddr.sin_family = AF_INET;
	MyAddr.sin_port = htons(1234);
	MyAddr.sin_addr.s_addr = INADDR_ANY;
	Serverfd = SOCKET(AF_INET, SOCK_STREAM, 0);

	BIND(Serverfd, (struct sockaddr *)&MyAddr, sizeof(MyAddr));

	LISTEN(Serverfd, 128);

	//
	fd_set set, oset; // set作为传入的监听集合，oset作为就绪集合，传入传出分离
	FD_ZERO(&set);
	FD_SET(Serverfd, &set);
	int readycode = 0;
	int maxfd = Serverfd;
	printf("The Select server 0.0.3 Accept Start\n");
	while (1)
	{
		oset = set;
		readycode = select(maxfd + 1, &oset, NULL, NULL, NULL);
		if (readycode > 0)
		{
			int i;
			while (readycode)
			{

				//辨别就绪
				// server_fd

				if (FD_ISSET(Serverfd, &oset))
				{ //使用server——fd在就绪集合oset中判断是否就绪

					bzero(recv_msg, sizeof(recv_msg));

					AddrLen = sizeof(ClientAddr);
					Clientfd = ACCEPT(Serverfd, (struct sockaddr *)&ClientAddr, &AddrLen);

					//储存socket

					for (i = 0; i < 1024; i++)
					{
						if (sock_list[i].sockfd == -1)
						{
							sock_list[i].sockfd = Clientfd;
							bzero(sock_list[i].client_ip, sizeof(16));
							inet_ntop(AF_INET, &ClientAddr.sin_addr.s_addr, sock_list[i].client_ip, 16);
							break;
						}
					}
					// set superviser
					FD_SET(Clientfd, &set);

					if (Clientfd > maxfd)
						maxfd = Clientfd;

					printf("ppid %d Client ip %s port %d connet successfully..\n", getpid(), sock_list[i].client_ip, ntohs(ClientAddr.sin_port));
					sprintf(send_msg, "hello %s welcome this server\n", sock_list[i].client_ip);
					send(Clientfd, &send_msg, sizeof(send_msg), 0);
					printf("Send Response");
					//
					FD_CLR(Serverfd, &oset);
				}
				else
				{
					// client_fd;
					for (i = 0; i < 1024; i++)
					{
						if (sock_list[i].sockfd != -1)
						{
							if (FD_ISSET(sock_list[i].sockfd, &oset))
							{
								// char Forward[2048];

								if ((len = recv(sock_list[i].sockfd, &Response, sizeof(Response), 0)) > 0)
								{

									if (strcmp(Response, "localtime\n") == 0)
									{
										bzero(time_buf, sizeof(time_buf));
										tp = time(NULL);
										ctime_r(&tp, time_buf); //计算时间
										printf("localtime Request compete\n");
										send(sock_list[i].sockfd, time_buf, strlen(time_buf), 0);
									}

									else
									{
										sprintf(Forward, "(%s) Say: %s\n\n", sock_list[i].client_ip, Response);
										printf("%s\n", Forward);
										int j;
										for (j = 0; j < 1024; j++)
										{
											if (sock_list[j].sockfd != -1)
											{
												send(sock_list[j].sockfd, Forward, strlen(Forward), 0);
											}
										}
									}
									bzero(Response, sizeof(Response));
									bzero(Forward, sizeof(Forward));
									FD_CLR(sock_list[i].sockfd, &oset);
								}
								else if (len == -1)
								{
									perror("Recv Call Error");
									exit(0);
								}
								else if (len == 0)
								{
									printf("Client_ip (%s) Exit, ", sock_list[i].client_ip);
									FD_CLR(sock_list[i].sockfd, &set);
									close(sock_list[i].sockfd);
									sock_list[i].sockfd = -1;
								}
								break;
							}
						}
					}
				}
				--readycode;
			}
		}
		else if (readycode == -1)
		{
			printf("Select Call Failed");
		}
	}
	return 0;
}

