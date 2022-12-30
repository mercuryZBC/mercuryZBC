#include<THREAD_SERVER.h>

pool_t* Server_Thread_Create(int Max, int Min, int Container_size){
    pool_t* ptr = NULL;
    if((ptr = (pool_t*)malloc(sizeof(pool_t))) == NULL){
        perror("pool_t malloc Failed");
    }   
    ptr->thread_shutdown = _TRUE;
    ptr->thread_max = Max;
    ptr->thread_min = Min;
    ptr->thread_alive = 0;
    ptr->thread_busy = 0;
    ptr->thread_exitcode = 0;
    if((ptr->Business_container = (task_t*)malloc(sizeof(task_t)*Container_size)) == NULL){
        perror("Business_container malloc Failed");
    }
    ptr->FRONT = 0;
    ptr->REAR = 0;
    ptr->CUR = 0;
    ptr->MAX = Container_size;
    if(pthread_mutex_init(&(ptr->lock),NULL) != 0 ||
    pthread_cond_init(&(ptr->Not_Full),NULL) != 0 ||
    pthread_cond_init(&(ptr->Not_Empty),NULL) != 0){
        perror("pthread lock or cond init Failed");
    }
    if((ptr->customer_tids = (pthread_t*)malloc(sizeof(pthread_t)*Max)) == NULL){
        perror("customer Thread malloc Failed");
    }
    bzero(ptr->customer_tids,sizeof(pthread_t) * Max);
    //预创建消费者线程
    int err;
    for(int i = 0; i < Min; i++){
        if((err = pthread_create(&(ptr->customer_tids[i]),NULL,Customer_Thread,(void*)ptr)) > 0){
            printf("Server_Thread_create, phtread_create Customer Failed %s",strerror(err));
        }
        else ++(ptr->thread_alive);
    }
    //创建管理者线程
    if((err = pthread_create(&(ptr->manager_tid),NULL,Manager_Thread,(void*)ptr)) > 0){
            printf("Server_Thread_create, phtread_create Manager Failed %s",strerror(err));
    }
    return ptr;
}//线程池初始化