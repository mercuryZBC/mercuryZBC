#include<THREAD_SERVER.h>


void* Manager_Thread(void* arg){
    pool_t* ptr = NULL;
    ptr = (pool_t*)arg;
    pthread_detach(pthread_self());
    printf("Manager Thread 0x%x, Running..\n",(unsigned int)pthread_self());
    int alive,busy,cur;
    int flag;
    int add;
    int err;
    while(ptr->thread_shutdown){
        pthread_mutex_lock(&ptr->lock);
        alive = ptr->thread_alive;
        busy = ptr->thread_busy;
        cur = ptr->CUR;
        printf("Manager Output Server Thread Information Alive[%d] Busy[%d] Idel[%d] Cur [%d] Busy-Alive[%.2f%%] Alive-All [%.2f%%]..\n",
        alive, busy, alive-busy, cur, (double)busy/alive, (double)(alive-busy)/alive);
        pthread_mutex_unlock(&ptr->lock);
    if((cur >= (alive - busy) || (double)(busy / alive) * 100 >= 70) && alive + _DEFCODE <= ptr->thread_max){
        for(flag = 0, add = 0;
            flag < ptr->thread_max && add < _DEFCODE;
            flag++){
                if(ptr->customer_tids[flag] == 0 || !IF_THREAD_ALIVE(ptr->customer_tids[flag])){
                    if((err = pthread_create(&(ptr->customer_tids[flag]), NULL, Customer_Thread, (void*)ptr)) > 0){
                        printf("Server Manager Thread 0x%x Create New Customer Failed %s\n",
                        (unsigned int)pthread_self(),strerror(err));
                        pthread_mutex_lock(&ptr->lock);
                        ++(ptr->thread_alive);
                        pthread_mutex_unlock(&ptr->lock);
                        ++add;
                    }            
                }
            }
    }
    if((busy * 2 < (alive - busy)) && ((alive - _DEFCODE) > ptr->thread_min)){
        pthread_mutex_lock(&ptr->lock);
        ptr->thread_exitcode = _DEFCODE;
        for(int i = 0; i <_DEFCODE; i++){
            pthread_cond_signal(&ptr->Not_Empty);//唤醒
        }
    }
    }
    pthread_exit(NULL);
}