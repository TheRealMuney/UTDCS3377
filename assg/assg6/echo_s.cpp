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

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 4) {
        fprintf(stderr, "Usage: %s <port1> [<port2> <port3>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port1 = atoi(argv[1]);
    int port2 = 0, port3 = 0;

    if (argc > 2) {
        port2 = atoi(argv[2]);
    }

    if (argc > 3) {
        port3 = atoi(argv[3]);
    }

        int listenfd1, listenfd2, listenfd3, connfd, udpfd, nready, maxfdp1;
        char buffer[MAXLINE];
        pid_t childpid;
        fd_set rset;
        ssize_t n;
        socklen_t len;
        const int on = 1;
        struct sockaddr_in cliaddr, servaddr, servaddr1, servaddr2, servaddr3;
        const char* message = "Hello, Client!\nEnter your message:\n";
        void sig_chld(int);

        /* create listening TCP socket */
        listenfd1 = socket(AF_INET, SOCK_STREAM, 0);
        bzero(&servaddr1, sizeof(servaddr1));
        servaddr1.sin_family = AF_INET;
        servaddr1.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr1.sin_port = htons(port1);
	
	if(port2 != 0) {
	/* create listening TCP socket */
	listenfd2 = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr2, sizeof(servaddr2));
	servaddr2.sin_family = AF_INET;
	servaddr2.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr2.sin_port = htons(port2);
	}
	if(port3 != 0) {
	/* create listening TCP socket */
	listenfd3 = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr3, sizeof(servaddr3));
	servaddr3.sin_family = AF_INET;
	servaddr3.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr3.sin_port = htons(port3);
	}


        // binding server addr structure to listenfd
        bind(listenfd1, (struct sockaddr*)&servaddr1, sizeof(servaddr1));
        listen(listenfd1, 3);
	if(port2 != 0) {
	bind(listenfd2, (struct sockaddr*)&servaddr2, sizeof(servaddr2));
	listen(listenfd2,3);
	if(port3 != 0) {
	bind(listenfd3, (struct sockaddr*)&servaddr3, sizeof(servaddr3));
	listen(listenfd3, 3);
	}
	}

        /* create UDP socket */
        udpfd = socket(AF_INET, SOCK_DGRAM, 0);
        // binding server addr structure to udp sockfd
        bind(udpfd, (struct sockaddr*)&servaddr1, sizeof(servaddr1));
	
	/* create UDP socket */
	int udpfd2 = socket(AF_INET, SOCK_DGRAM, 0);
	// binding server addr structure to udp sockfd
	bind(udpfd2, (struct sockaddr*)&servaddr2, sizeof(servaddr2));         
	
	/* create UDP socket */
        int udpfd3 = socket(AF_INET, SOCK_DGRAM, 0);
        // binding server addr structure to udp sockfd
        bind(udpfd3, (struct sockaddr*)&servaddr3, sizeof(servaddr3));

	
        // clear the descriptor set
        FD_ZERO(&rset);

        // get maxfd
        maxfdp1 = max(listenfd1, udpfd) + 1;
        int maxfdp2 = max(listenfd2, udpfd2) + 1;
	int maxfdp3 = max(listenfd3, udpfd3) + 1;
      
	if(port2 != 0) {
		FD_SET(listenfd2, &rset);
                FD_SET(udpfd2, &rset);	
		}
  	if(port3 != 0) {
		FD_SET(listenfd3, &rset);
                FD_SET(udpfd3, &rset);
  		}

	//childpid2 = fork();
	//childpid3 = fork();
	
	  /* create a UDP socket */
                        int udpfdLog = socket(AF_INET, SOCK_DGRAM, 0);
                        if (udpfdLog < 0) {
                              perror("UDP socket creation failed");
                              exit(EXIT_FAILURE);
                        }

                        /* set up the server address and port */
                        struct sockaddr_in server_addr4;
                        bzero(&server_addr4, sizeof(server_addr4));
                        server_addr4.sin_family = AF_INET;
                        server_addr4.sin_port = htons(PORT2LOG); // use log port 9999
			server_addr4.sin_addr.s_addr = inet_addr("10.176.92.17");

	int childpid2, childpid3;
	
	if( port3 != 0 && (childpid3 = fork()) == 0) {
	

       for (;;) {

                // set listenfd and udpfd in readset
                FD_SET(listenfd3, &rset);
                FD_SET(udpfd3, &rset);
		
		connfd = 0;
                // select the ready descriptor
                nready = select(maxfdp3, &rset, NULL, NULL, NULL);
                // if tcp socket is readable then handle
                // it by accepting the connection
                if (FD_ISSET(listenfd3, &rset)) {
			len = sizeof(cliaddr);
			connfd = accept(listenfd3, (struct sockaddr*)&cliaddr, &len);
			int nbytes;
			for(;;) {
			
			//clear buffer and recv message 
                        memset(buffer, 0, sizeof(buffer));
			nbytes = recv(connfd, buffer, sizeof(buffer), 0);
			send(connfd, buffer, nbytes, 0); //echo

                        /* send the message to the log server */
                        ssize_t bytes_sent = sendto(udpfdLog, buffer, strlen(buffer)+1, 0, (struct sockaddr*)&server_addr4, sizeof(server_addr4));

                        if (bytes_sent < 0) {//if sendto log failed
                                         exit(1);
                            } else {
                                        printf("Message sent to log server: %s", buffer);
                                }
			
			if(strncmp(buffer, "exit", 4) == 0) {//if exit
				close(udpfdLog);
				break;
			}
                        
			}//inside for loop close
			FD_CLR(listenfd3, &rset);
                        close(connfd);
			close(listenfd3);
			
                }//if FDISET close
		
                // if udp socket is readable receive the message.
                if (FD_ISSET(udpfd3, &rset)) {
                        len = sizeof(cliaddr);
                        memset(buffer, 0, sizeof(buffer));
                        printf("Message from UDP client: ");
                        n = recvfrom(udpfd3, buffer, sizeof(buffer), 0,
                                                (struct sockaddr*)&cliaddr, &len);
                        printf(buffer);
                        sendto(udpfd3, buffer, sizeof(buffer), 0,
                        (struct sockaddr*)&cliaddr, sizeof(cliaddr));
			
                        /* send the message to the server */
                        ssize_t bytes_sent = sendto(udpfdLog, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr4, sizeof(server_addr4));

                        if (bytes_sent < 0) {
                                         perror("sendto() failed");
                                         exit(EXIT_FAILURE);
                            } else {
                                        printf("Message sent to server: %s", buffer);
                                }

                                /* close the socket */
                                close(udpfdLog);
                                close(n);
                                exit(0);
                        }

                }

	}

	if( port2 != 0 && (childpid2 = fork()) == 0) {
		
	for(;;) {


                // set listenfd and udpfd in readset
                FD_SET(listenfd2, &rset);
                FD_SET(udpfd2, &rset);
		
		connfd = 0;
                // select the ready descriptor
                nready = select(maxfdp2, &rset, NULL, NULL, NULL);
                // if tcp socket is readable then handle
                // it by accepting the connection
                if (FD_ISSET(listenfd2, &rset)) {
			len = sizeof(cliaddr);
			connfd = accept(listenfd2, (struct sockaddr*)&cliaddr, &len);
			int nbytes;
			for(;;) {
			
			//clear buffer and recv message 
                        memset(buffer, 0, sizeof(buffer));
			nbytes = recv(connfd, buffer, sizeof(buffer), 0);
			send(connfd, buffer, nbytes, 0); //echo

                        /* send the message to the log server */
                        ssize_t bytes_sent = sendto(udpfdLog, buffer, strlen(buffer)+1, 0, (struct sockaddr*)&server_addr4, sizeof(server_addr4));

                        if (bytes_sent < 0) {//if sendto log failed
                                         exit(1);
                            } else {
                                        printf("Message sent to log server: %s", buffer);
                                }
			
			if(strncmp(buffer, "exit", 4) == 0) {//if exit
				close(udpfdLog);
				break;
			}
                        
			}//inside for loop close
			FD_CLR(listenfd2, &rset);
                        close(connfd);
			close(listenfd2);
			
                }//if FDISET close
		
                // if udp socket is readable receive the message.
                if (FD_ISSET(udpfd2, &rset)) {
                        len = sizeof(cliaddr);
                        memset(buffer, 0, sizeof(buffer));
                        printf("Message from UDP client: ");
                        n = recvfrom(udpfd2, buffer, sizeof(buffer), 0,
                                                (struct sockaddr*)&cliaddr, &len);
                        printf(buffer);
                        sendto(udpfd2, buffer, sizeof(buffer), 0,
                        (struct sockaddr*)&cliaddr, sizeof(cliaddr));
			
                        /* send the message to the server */
                        ssize_t bytes_sent = sendto(udpfdLog, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr4, sizeof(server_addr4));

                        if (bytes_sent < 0) {
                                         perror("sendto() failed");
                                         exit(EXIT_FAILURE);
                            } else {
                                        printf("Message sent to server: %s", buffer);
                                }

                                /* close the socket */
                                close(udpfdLog);
                                close(n);
                                exit(0);
                        }

                }


	}

       for (;;) {

                // set listenfd and udpfd in readset
                FD_SET(listenfd1, &rset);
                FD_SET(udpfd, &rset);
		
		connfd = 0;
                // select the ready descriptor
                nready = select(maxfdp1, &rset, NULL, NULL, NULL);
                // if tcp socket is readable then handle
                // it by accepting the connection
                if (FD_ISSET(listenfd1, &rset)) {
			len = sizeof(cliaddr);
			connfd = accept(listenfd1, (struct sockaddr*)&cliaddr, &len);
			int nbytes;
			for(;;) {
			
			//clear buffer and recv message 
                        memset(buffer, 0, sizeof(buffer));
			nbytes = recv(connfd, buffer, sizeof(buffer), 0);
			send(connfd, buffer, nbytes, 0); //echo

                        /* send the message to the log server */
                        ssize_t bytes_sent = sendto(udpfdLog, buffer, strlen(buffer)+1, 0, (struct sockaddr*)&server_addr4, sizeof(server_addr4));

                        if (bytes_sent < 0) {//if sendto log failed
                                         exit(1);
                            } else {
                                        printf("Message sent to log server: %s", buffer);
                                }
			
			if(strncmp(buffer, "exit", 4) == 0) {//if exit
				close(udpfdLog);
				break;
			}
                        
			}//inside for loop close
			FD_CLR(listenfd1, &rset);
                        close(connfd);
			close(listenfd1);
			
                }//if FDISET close
		
                // if udp socket is readable receive the message.
                if (FD_ISSET(udpfd, &rset)) {
                        len = sizeof(cliaddr);
                        memset(buffer, 0, sizeof(buffer));
                        printf("Message from UDP client: ");
                        n = recvfrom(udpfd, buffer, sizeof(buffer), 0,
                                                (struct sockaddr*)&cliaddr, &len);
                        printf(buffer);
                        sendto(udpfd, buffer, sizeof(buffer), 0,
                        (struct sockaddr*)&cliaddr, sizeof(cliaddr));
			
                        /* send the message to the server */
                        ssize_t bytes_sent = sendto(udpfdLog, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr4, sizeof(server_addr4));

                        if (bytes_sent < 0) {
                                         perror("sendto() failed");
                                         exit(EXIT_FAILURE);
                            } else {
                                        printf("Message sent to server: %s", buffer);
                                }

                                /* close the socket */
                                close(udpfdLog);
                                close(n);
                                exit(0);
                        }

                }//outside for loop close
	exit(0);
        }//main close

