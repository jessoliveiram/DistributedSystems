#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define LEN 4096
#define SERVER_ADDR "127.0.0.1"


int main(int argc, char *argv[]) {}

int connect() {
    struct sockaddr_in server;
    int sockfd;

    fprintf(stdout, "Starting client...\n");

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error to create socket");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Client with socket fd %d created\n", sockfd);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Error to connect");
        return EXIT_FAILURE;
    }
    return sockfd;
}

int disconnect(int sockfd) {
    close(sockfd);
    fprintf(stdout, "\nConnection closed\n");
    return EXIT_SUCCESS;
}

void converse(int sockfd) {
    char buffer[LEN];
    int slen;
    int ok = true;

    while(ok) {
        fprintf(stdout, "Sending REQUEST to server...\n");
        strcpy(buffer, "REQUEST");
        send(sockfd, buffer, strlen(buffer), 0);

        slen = recv(sockfd, buffer, LEN, 0);
        printf("Server: %s\n", buffer);
        if (strcmp(buffer, "GRANT") == 0) {
            printf("Write arquivo.txt\n");
            ok = false;
        }
    }
    fprintf(stdout, "Send RELEASE to server\n");
    strcpy(buffer, "RELEASE");
    send(sockfd, buffer, strlen(buffer), 0);
}