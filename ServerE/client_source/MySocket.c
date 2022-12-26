#include<MySocket.h>


int SOCKET(int domain, int type, int protocol){
	int sockfd;
	if((sockfd = socket(domain,type,protocol)) == -1){
		perror("Socket Create Failed");

		return sockfd;
}
	printf("Scoket create successful\n");
	return sockfd;
}


int BIND(int sockfd, const struct sockaddr* addr, socklen_t addrlen){
	int reveal;
	if((reveal = bind(sockfd, addr, addrlen)) == -1){
		perror("BIND Call Failed");
		return reveal;
	}
	printf("BIND Successfully\n");
	return reveal;
}


int LISTEN(int sockfd, int backlog){
	
	int reveal;
	if((reveal = listen(sockfd, backlog)) == -1){
		perror("LISTEN Call Failed");
		return reveal;
	}
	printf("LISTEN Successfully\n");
	return reveal;
}

int ACCEPT(int sockfd, struct sockaddr *addr , socklen_t* addrlen){
	
	int reveal;
	if((reveal = accept(sockfd, addr, addrlen)) == -1){
		perror("ACCEPT Call Failed");
		return reveal;
	}
	printf("Accpet Successfully\n");
	return reveal;

}


int CONNECT(int sockfd, const struct sockaddr* addr, socklen_t addrlen){
	
	int reveal;
	if((reveal = connect(sockfd, addr, addrlen)) == -1){
		perror("CONNECT Call Failed");
		return reveal;
	}
	printf("connect Successfully\n");
	return reveal;
}
