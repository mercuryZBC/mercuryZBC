#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<signal.h>
#include<error.h>
#include<unistd.h>
#include<sys/mman.h>

pthread_cond_t buf_A_Empty;
pthread_cond_t buf_A_nEmpty;
pthread_cond_t buf_B_Empty;
pthread_cond_t buf_B_nEmpty;
pthread_mutex_t LA;
pthread_mutex_t LB;


char bufA[1024];
char bufB[1024];
int flag = 0;


void* T_A(void* arg){
	pthread_detach(pthread_self());
	int nLen;
	FILE* fd = fopen("./ERROR.log","r");
	while(1){
	  pthread_mutex_lock(&LA);
		while(strlen(bufA) != 0) pthread_cond_wait(&buf_A_nEmpty, &LA);// file not Empty;
	 	printf("Thread_A 0x%x is working\n", (unsigned int)pthread_self());	
		fgets(bufA, sizeof(bufA),fd);
		nLen = strlen(bufA);
		if(nLen == 0) {
			fclose(fd);
			flag = 1;
	 		printf("Thread_A 0x%x will exit\n", (unsigned int)pthread_self());
			pthread_mutex_unlock(&LA);
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&LA);
		pthread_cond_signal(&buf_A_Empty);
	}

}

void* T_B(void* arg){

	pthread_detach(pthread_self());
	while(1){
	  	pthread_mutex_lock(&LA);
		if(flag){
				printf("Thread_B 0x%x will exit\n", (unsigned int)pthread_self());
				//pthread_mutex_unlock(&LB);
				pthread_exit(NULL);
		while(strlen(bufA) == 0){ 
				
			  pthread_cond_wait(&buf_A_Empty, &LA);
			  
			  
		} 
			  
		}
	 	printf("Thread_B 0x%x is working\n", (unsigned int)pthread_self());	
		if(strstr(bufA,"E CHIUSECASE") ||strstr(bufA, "E CamX")){
			pthread_mutex_lock(&LB);
			while(strlen(bufB) != 0) {
				pthread_cond_wait(&buf_B_nEmpty, &LB);
	 			}
			memcpy(bufB,bufA, sizeof(bufA));
			pthread_mutex_unlock(&LB);
			pthread_cond_signal(&buf_B_Empty);
		}
		bzero(bufA, sizeof(bufA));
		pthread_mutex_unlock(&LA);
		pthread_cond_signal(&buf_A_nEmpty);
	}

}


void* T_C(void* arg){
	pthread_detach(pthread_self());
	int Dfd = open("Result.log", O_WRONLY|O_CREAT,0664);
	while(1){
	  	pthread_mutex_lock(&LB);
		
		while(strlen(bufB) == 0){ 
			
			pthread_cond_wait(&buf_B_Empty, &LB);
		}
		if(flag){
			printf("Thread_C 0x%x will exit\n", (unsigned int)pthread_self());
			pthread_exit(NULL);
		} 
	 	printf("Thread_C 0x%x is working\n", (unsigned int)pthread_self());	
		
		write(Dfd, bufB, strlen(bufB));
		bzero(bufB,sizeof(bufB));
		pthread_mutex_unlock(&LB);
		pthread_cond_signal(&buf_B_nEmpty);

}

}

int main(void){
	
	if(pthread_cond_init(&buf_A_Empty, NULL) != 0 || pthread_cond_init(&buf_A_nEmpty, NULL) != 0 || 
		pthread_cond_init(&buf_B_Empty,NULL) != 0 || pthread_cond_init(&buf_B_nEmpty, NULL) != 0 || 
		pthread_mutex_init(&LA, NULL) != 0 || pthread_mutex_init(&LB,NULL) != 0){
		
		
			printf("cond or lock init failed..\n");
			exit(0);
	}


	pthread_t tids[3];
	pthread_create(&tids[0],NULL,T_A,NULL);
	pthread_create(&tids[1],NULL,T_B,NULL);
	pthread_create(&tids[2],NULL,T_C,NULL);
	

	while(1) sleep(1);
	return 0;
}
