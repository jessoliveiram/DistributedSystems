#include <iostream>
#include <fstream>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../utils/error.h"
#include "../utils/message.h"


#define PORT 8080
#define BUFFER_SIZE 20
#define SERVER_ADDR "127.0.0.1"

using namespace std;

int connect() {
    struct sockaddr_in serveraddr;
    int sockfd;

    cout << "Start the UDP Client..." << endl;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        error((char *) "socket creation failed");
    }
    fprintf(stdout, "Client with socket fd %d created\n", sockfd);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
        error((char *) "ERROR on binding");

    return sockfd;
}

void disconnect(int sockfd) {
    close(sockfd);
    fprintf(stdout, "Connection closed\n");
}

void converse(int sockfd, int n_repeat) {
    char buffer[BUFFER_SIZE];
    std::string request;
    std::string release;

    while (n_repeat >= 0) {
        fprintf(stdout, "Sending REQUEST to server...\n");
        request = encode_message(MESSAGE_REQUEST, BUFFER_SIZE);
        send(sockfd, request.c_str(), strlen(request.c_str()), 0);

        recv(sockfd, buffer, BUFFER_SIZE, 0);
        printf("Server: %s\n", buffer);
        //decode message from buffer and compare after this
        if (strcmp(buffer, "GRANT") == 0) {
            printf("Write arquivo.txt\n");
            // check error
            n_repeat -= n_repeat;
            fprintf(stdout, "Send RELEASE to server\n");
            release = encode_message(MESSAGE_RELEASE, BUFFER_SIZE);
            send(sockfd, release.c_str(), strlen(release.c_str()), 0);
        }
    }
}

int write_file() {
    ofstream myfile("result.txt");
    myfile << "PID: " << std::to_string(getpid());
    auto now = std::chrono::system_clock::now();
    time_t time_now = std::chrono::system_clock::to_time_t(now);
    myfile << "HORA: " << time_now << "\n";
    myfile.close();
    return 0;
}

int main(int argc, char *argv[]) {
    int n_repeat = 5;
    int sockfd = connect();
    converse(sockfd, n_repeat);
    disconnect(sockfd);
}