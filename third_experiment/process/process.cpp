#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../utils/error.h"
#include "../utils/message.h"


#define processaddr_PORT 8080
#define serveraddr_PORT 56820
#define BUFFER_SIZE 20
#define serveraddr_ADDR "127.0.0.1"

using namespace std;

int connect() {
    struct sockaddr_in processaddr;
    int sockfd;

    cout << "Start the UDP Client..." << endl;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
        error((char *) "ERROR socket creation failed");
    
    cout << "client with socketfd " << sockfd << " created\n" << endl;

    processaddr.sin_family = AF_INET;
    processaddr.sin_port = htons(processaddr_PORT);
    processaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(sockfd, (struct sockaddr *) &processaddr, sizeof(processaddr)) < 0)
        error((char *) "ERROR on binding");

    return sockfd;
}

void disconnect(int sockfd) {
    close(sockfd);
    cout << "connection closed\n" << endl;
}

int write_file() {
    std::ofstream myfile;
    myfile.open("result.txt", std::ios_base::app);
    myfile << "PID: " << std::to_string(getpid()) << " ";
    auto now = std::chrono::system_clock::now();
    time_t time_now = std::chrono::system_clock::to_time_t(now);
    myfile << "HORA: " << time_now << "\n";
    myfile.close();
    return 0;
}

void converse(int sockfd, int n_repeat) {
    char buffer[BUFFER_SIZE];
    std::string request, release;
    int n;

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(serveraddr_PORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    while (n_repeat > 0)
    {
        cout << "sending REQUEST to serveraddr...\n" << endl;

        request = encode_message(MESSAGE_REQUEST, BUFFER_SIZE);
        n = sendto(sockfd, request.c_str(), strlen(request.c_str()), 0,
                   (struct sockaddr *) &serveraddr, sizeof(serveraddr));
        if (n < 0)
            error((char*)"ERROR in sendto");

        n = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (n < 0)
            error((char*)"ERROR in recv");

        cout << "serveraddr response: %s\n" << buffer << endl;

        message msg = decode_message(buffer);
        if(msg.valid) {
            cout << msg.message_type << endl;
            if (strcmp(msg.message_type, MESSAGE_GRANT) == 0) {

                cout << "writing in result.txt\n" << endl;

                write_file();
                if (n < 0)
                    error((char*)"ERROR writing file");
                n_repeat -= 1;

                cout << "send RELEASE to serveraddr\n" << endl;

                release = encode_message(MESSAGE_RELEASE, BUFFER_SIZE);
                n = sendto(sockfd, release.c_str(), strlen(release.c_str()), 0,
                           (struct sockaddr *) &serveraddr, sizeof(serveraddr));
                if (n < 0)
                    error((char*)"ERROR in sendto");
            }
            else {
                cout << "invalid message" << endl;
            }
        }
        else {
            cout << "invalid message" << endl;
        }
    }
}

int main(int argc, char *argv[]) {
    int n_repeat = 5; //argv[1];
    int sockfd = connect();
    converse(sockfd, n_repeat);
    disconnect(sockfd);
}