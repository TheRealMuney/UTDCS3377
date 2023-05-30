// Server program using geeks for geeks template
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <unistd.h>
#define PORT2SERVER 8189
#define PORT2LOG 9999
#define MAXLINE 1024

int max(int x, int y)
{
	if (x > y)
		return x;
	else
		return y;
}

int main()
{
	int listenfd, connfd, udpfd, nready, maxfdp1;
	char buffer[MAXLINE];
	pid_t childpid;
	fd_set rset;
	ssize_t n;
	socklen_t len;
	const int on = 1;
	struct sockaddr_in cliaddr, servaddr;
	char* message = "Hello, Client!\nEnter your message:\n";
	void sig_chld(int);

	/* create listening TCP socket */
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT2SERVER);

	// binding server addr structure to listenfd
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	listen(listenfd, 10);

	/* create UDP socket */
	udpfd = socket(AF_INET, SOCK_DGRAM, 0);
	// binding server addr structure to udp sockfd
	bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	// clear the descriptor set
	FD_ZERO(&rset);

	// get maxfd
	maxfdp1 = max(listenfd, udpfd) + 1;
	for (;;) {

		// set listenfd and udpfd in readset
		FD_SET(listenfd, &rset);
		FD_SET(udpfd, &rset);

		// select the ready descriptor
		nready = select(maxfdp1, &rset, NULL, NULL, NULL);

		// if tcp socket is readable then handle
		// it by accepting the connection
		if (FD_ISSET(listenfd, &rset)) {
			len = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
			if ((childpid = fork()) == 0) {
				close(listenfd);
				bzero(buffer, sizeof(buffer));
				read(connfd, buffer, sizeof(buffer));
				/*udpfdLog = socket(AF_INET, SOCK_DGRAM, 0);
				bind(udpfdLog, (struct sockaddr*)&servaddr, sizeof(servaddr));
				sendto(udpfdLog, message, sizeof(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
				*/
			/* create a UDP socket */
			int udpfdLog = socket(AF_INET, SOCK_DGRAM, 0);
			if (udpfdLog < 0) {
			      perror("UDP socket creation failed");
			      exit(EXIT_FAILURE);
			}

			/* set up the server address and port */
			struct sockaddr_in server_addr;
			bzero(&server_addr, sizeof(server_addr));
			server_addr.sin_family = AF_INET;
			server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // use any available interface
			server_addr.sin_port = htons(PORT2LOG); // use port 8188

			server_addr.sin_addr.s_addr = inet_addr("10.176.92.15");

			/* send the message to the server */
			ssize_t bytes_sent = sendto(udpfdLog, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
			
			if (bytes_sent < 0) {
					 perror("sendto() failed");
					 exit(EXIT_FAILURE);
			    } else {
				        printf("Message sent to server: %s", message);
				}

				/* close the socket */
				close(udpfdLog);
				write(connfd, (const char*)buffer, sizeof(buffer));
				close(connfd);
				exit(0);
			}
			close(connfd);
		}
		// if udp socket is readable receive the message.
		if (FD_ISSET(udpfd, &rset)) {
			len = sizeof(cliaddr);
			bzero(buffer, sizeof(buffer));
			printf("\nMessage from UDP client: ");
			n = recvfrom(udpfd, buffer, sizeof(buffer), 0,
						(struct sockaddr*)&cliaddr, &len);
			puts(buffer);
			sendto(udpfd, (const char*)message, sizeof(buffer), 0,
			(struct sockaddr*)&cliaddr, sizeof(cliaddr));
		}
	}
}

