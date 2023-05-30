#include <stdio.h>
#include <iostream>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX 80
#define SA struct sockaddr

using namespace std;

void func(int sockfd)
{
	char buff[MAX];
	int n;
	int leng;
	bzero(buff, sizeof(buff));
	for (;;) {
		usleep(500000);
		printf("Enter your message: ");
		n = 0;
		memset(buff, 0, sizeof(buff));
		while ((buff[n++] = getchar()) != '\n')
			;
		send(sockfd, buff, strlen(buff)+1 ,0);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
		leng = strlen(buff) +1;
		memset(buff, 0, sizeof(buff));
		
		recv(sockfd, buff, leng, 0);
		printf("From Server : %s", buff);
		
	}
}

int main(int argc, char *argv[])
{
	if(argc != 4) {
	printf("Not enough arguments");
	exit(0);
	}
	
	char* clientType = argv[1];
	char* ipAd = argv[2];
	int port = atoi(argv[3]);

	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
	
	if(strcmp(clientType, "t") == 0) {

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ipAd);
	servaddr.sin_port = htons(port);
	
	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}
	else if(strcmp(clientType, "u") == 0){

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
	printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ipAd);
	servaddr.sin_port = htons(port);
	
	char buffer[MAX];
	char message[MAX];
	int len;

	printf("Enter your message: ");
	fgets(message, sizeof(message), stdin);
	
	// connect the client socket to server socket
	sendto(sockfd, message, strlen(message), 0, (const struct sockaddr*)&servaddr, sizeof(servaddr));
	
	printf("Server:");
	
	int n = recvfrom(sockfd, (char*)buffer, MAX, 0, (struct sockaddr*)&servaddr, (socklen_t*)&len);
	puts(buffer);
	
	// close the socket
	close(sockfd);
}
	else {
	printf("Retry with u for UDP or t for TCP");
	}
	exit(0);
}
