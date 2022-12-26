#include<THREAD_SERVER.h>

int TF_THREAD_ALIVE(pthread_t tid){
    int err;
    err = pthread_kill(tid, 0);
    if(err == ESRCH){
        return 0;
    }
    return 1;
}