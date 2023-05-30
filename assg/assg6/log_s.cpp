#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFLEN 1024
#define PORT 9999 

void remove_newline(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

int main(void) {
    //printf("yay");
    struct sockaddr_in server_addr, client_addr;
    int sockfd, recvlen;
    socklen_t client_len = sizeof(client_addr);
    char buf[BUFLEN];
    char timestamp[BUFLEN];
    time_t t;
    FILE *logfile;
    //printf("made it"); 
    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }
    
    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    
    // Bind socket to server address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }
    
    // Open the log file
    char* homedir = getenv("HOME");
    const char* file_path = "/learn/assg/assg6/echo.log";
    char full_path[strlen(homedir) + strlen(file_path) + 1];
    
    strcpy(full_path, homedir);
    strcat(full_path, file_path);

    logfile = fopen(full_path, "a");
    if (!logfile) {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming messages
    while (1) {
        recvlen = recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr *)&client_addr, &client_len);
        if (recvlen < 0) {
            perror("Failed to receive message");
            continue;
        }
        
	remove_newline(buf);

        // Get the current timestamp
        t = time(NULL);
        strftime(timestamp, BUFLEN, "%Y-%m-%d %H:%M:%S", localtime(&t));
        
        // Log the message to the file
        //printf("here");
	fprintf(logfile, "%s: %s was received from %s", timestamp, buf, inet_ntoa(client_addr.sin_addr));
	fprintf(logfile, "\n");
        fflush(logfile);
    }
    
    // Close the log file and socket
    fclose(logfile);
    close(sockfd);
    
    return 0;
}

