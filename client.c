#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[512];

    if (argc < 2) {
        fprintf(stderr, "usage: %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        error("[!]ERROR opening socket");
    }

    server = gethostbyname(argv[1]);

    if (server == NULL) {
        error("[!] no such host");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error([!]ERROR connecting);
    }

    printf("Please enter the message: ");
    bzero(buffer, 512);
    fgets(buffer, 511, stdin);
    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0) {
        error("[!]ERROR writing to socket");
    }

    bzero(buffer, 512);
    n = read(sockfd, buffer, 511);
    
    if (n < 0) {
        error("[!]ERROR reading from socket");
    }

    printf("%s\n", buffer);

    close(sockfd);
    return 0;
}
