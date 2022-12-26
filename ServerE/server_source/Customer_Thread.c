#include<THREAD_SERVER.h>


void* Customer_Thread(void* arg){
    pool_t* ptr = NULL;
    ptr = (pool_t *)arg;
    pthread_detach(pthread_self());
    task_t bs;
    printf("Server Customer 0x%x Waiting Business...\n",(unsigned int)pthread_self());
    while(ptr->thread_shutdown){
        pthread_mutex_lock(&ptr->lock);
        while(ptr->CUR == 0){
            pthread_cond_wait(&(ptr->Not_Empty), &ptr->lock);
            if(ptr->thread_shutdown == 0){
                pthread_mutex_unlock(&ptr->lock);
                printf("Server customer Thread 0x%x, shutdown %d, Exit...\n",
                (unsigned int)pthread_self(),ptr->thread_shutdown);
                pthread_exit(NULL);
            }
            if(ptr->thread_exitcode > 0){
                --(ptr->thread_exitcode);
                --(ptr->thread_alive);
                pthread_mutex_unlock(&ptr->lock);
                printf("Customer Thread 0x%x, exitcode -1 = %d, Exiting..\n",
                (unsigned int)pthread_self(),ptr->thread_exitcode);
                pthread_exit(NULL);
            }
        }
        bs.Business = ptr->Business_container[ptr->REAR].Business;
        bs.Business_arg = ptr->Business_container[ptr->REAR].Business_arg;
        --(ptr->CUR);
        ptr->REAR = (ptr->REAR + 1) % ptr->thread_max;
        ++(ptr->thread_busy);
        pthread_mutex_unlock(&ptr->lock);
        pthread_cond_signal(&ptr->Not_Full);
        bs.Business(bs.Business_arg);//业务执行不能在锁中
        pthread_mutex_lock(&ptr->lock);
        --(ptr->thread_busy);
        pthread_mutex_unlock(&ptr->lock);
    }
    pthread_exit(NULL);
}