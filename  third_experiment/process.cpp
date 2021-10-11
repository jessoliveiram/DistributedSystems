#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define LEN 4096
#define SERVER_ADDR "127.0.0.1"


int main(int argc, char *argv[]) {
    struct sockaddr_in server;
    int sockfd;
    int slen;

    char buffer[LEN];

    fprintf(stdout, "Starting Client ...\n");

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error on client socket creation:");
        return -1;
    }
    fprintf(stdout, "Client socket created with fd: %d\n", sockfd);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("Can't connect to server");
        return -1;
    }

    if ((slen = recv(sockfd, buffer, LEN, 0)) > 0) {
        buffer[slen + 1] = '\0';
        fprintf(stdout, "Server says: %s\n", buffer);
    }

    close(sockfd);
    fprintf(stdout, "\nConnection closed\n\n");
    return 0;
}