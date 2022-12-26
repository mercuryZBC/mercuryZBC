#pragma once
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<errno.h>

#define _SERVER_IP "192.168.174.135"
#define _SERVER_PORT 8080
#define _TRUE 1
#define _FALSE 0
#define _BACKLOG 128
#define _BUFSIZE 1500
#define _IPSIZE 16
#define _DEFCODE 10
#define _EPOLL_MAX 10000

int Epfd; //监听树描述符
int Sock_array[_EPOLL_MAX];
pthread_mutex_t array_lock;
pthread_mutex_t accept_lock;
//Business struct
typedef struct{
    void *(*Business)(void*);
    void * Business_arg;
}task_t;

//thread_pool_struct
typedef struct{
    int thread_shutdown;
    int thread_max;
    int thread_min;
    int thread_alive;
    int thread_busy;
    int thread_exitcode;


    task_t* Business_container;
    int FRONT;
    int REAR;
    int CUR;
    int MAX;


    pthread_cond_t Not_Full;
    pthread_cond_t Not_Empty;
    pthread_mutex_t lock;
    pthread_t* customer_tids;
    pthread_t manager_tid;
}pool_t;


int Server_Start();
int Server_Net_INIT();//服务器网络初始化
pool_t* Server_Thread_Create(int Max, int Min, int Container_size);//线程池初始化
int Server_Thread_Destroy(pool_t*);
void* Customer_Thread(void* arg);
void* Manager_Thread(void* arg);
int Epoll_Create(int sockfd);
int Epoll_Listen(int sockfd, pool_t* ptr);
int Producer_Add_Task(pool_t* ptr, task_t bs);
void* Business_Accept(void* arg);
void* Business_Date(void* arg);
int IF_THREAD_ALIVE(pthread_t tid);