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
#include <poll.h>

struct pollfd listen_array[4096];

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
	bzero(Response, sizeof(Response));
	bzero(Forward, sizeof(Forward));
	bzero(send_msg, sizeof(send_msg));
	for (int i = 0; i < 4096; i++)
	{
		listen_array[i].fd = -1;
	}

	bzero(&MyAddr, sizeof(MyAddr));
	MyAddr.sin_family = AF_INET;
	MyAddr.sin_port = htons(1234);
	MyAddr.sin_addr.s_addr = INADDR_ANY;
	Serverfd = SOCKET(AF_INET, SOCK_STREAM, 0);

	BIND(Serverfd, (struct sockaddr *)&MyAddr, sizeof(MyAddr));

	LISTEN(Serverfd, 128);

	printf("The POLL server 0.0.3 Accept Start\n");
	listen_array[0].fd = Serverfd;
	listen_array[0].events = POLLIN;
	while (1)
	{

		int readycode = poll(listen_array, 1024, -1); //阻塞
		if (readycode > 0)
		{
			int i;
			while (readycode)
			{

				//辨别就绪
				// server_fd

				if (listen_array[0].revents == POLLIN)
				{

					AddrLen = sizeof(ClientAddr);
					Clientfd = ACCEPT(Serverfd, (struct sockaddr *)&ClientAddr, &AddrLen);

					//储存socket

					for (i = 1; i < 1024; i++)
					{
						if (listen_array[i].fd == -1)
						{
							listen_array[i].fd = Clientfd;
							listen_array[i].events = POLLIN;
							break;
						}
					}

					printf("pid %d Client_sock %d port %d connet successfully..\n", getpid(), listen_array[i].fd, ntohs(ClientAddr.sin_port));

					sprintf(send_msg, "welcome the server\n");
					send(Clientfd, &send_msg, sizeof(send_msg), 0);
					printf("Send Response");
					//
					listen_array[0].revents = 0;
				}
				else
				{
					// client_fd;

					for (i = 1; i < 1024; i++)
					{
						if (listen_array[i].fd != -1)
						{
							if (listen_array[i].revents == POLLIN)
							{
								// char Forward[2048];

								if ((len = recv(listen_array[i].fd, &Response, sizeof(Response), 0)) > 0)
								{

									if (strcmp(Response, "localtime\n") == 0)
									{
										bzero(time_buf, sizeof(time_buf));
										tp = time(NULL);
										ctime_r(&tp, time_buf); //计算时间
										printf("localtime Request compete\n");
										send(listen_array[i].fd, time_buf, strlen(time_buf), 0);
									}

									else
									{
										// sprintf(Forward, (%c) Say: %s\n\n", listen_array[i].fd, Response);

										sprintf(Forward, "xxx say %s", Response);
										printf("%s\n", Forward);
										int j;
										for (j = 1; j < 1024; j++)
										{
											if (listen_array[j].fd != -1)
											{
												send(listen_array[j].fd, Forward, strlen(Forward), 0);
											}
										}
									}
									bzero(Response, sizeof(Response));
									bzero(Forward, sizeof(Forward));
									listen_array[i].revents = 0;
								}
								if (len == -1)
								{
									perror("Recv Call Error");
									exit(0);
								}
								else if (len == 0)
								{
									listen_array[i].revents = 0;
									close(listen_array[i].fd);
									listen_array[i].fd = -1;
								}
							}
						}
					}
				}
				--readycode;
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
