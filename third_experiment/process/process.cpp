#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../utils/error.h"
#include "../utils/message.h"


#define SERVERADDR_PORT 8080
#define BUFFER_SIZE 20

using namespace std;

// create a socket and return the socketfd number
int connect(int process_port) {
    struct sockaddr_in processaddr;
    int sockfd;

    cout << "Start the UDP Client..." << endl;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
        error((char *) "ERROR socket creation failed");
    
    cout << "client with socketfd " << sockfd << " created\n" << endl;

    // config connection
    processaddr.sin_family = AF_INET;
    processaddr.sin_port = htons(process_port);
    processaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(sockfd, (struct sockaddr *) &processaddr, sizeof(processaddr)) < 0)
        error((char *) "ERROR on binding");

    return sockfd;
}

// close socket connection
void disconnect(int sockfd) {
    close(sockfd);
    cout << "connection closed\n" << endl;
}

// write PID and Time to result.txt file
// after that, sleep for n seconds
int write_file(int sleep_seconds) {
    std::ofstream myfile;
    myfile.open("result.txt", std::ios_base::app);
    myfile << "PID: " << std::to_string(getpid()) << " | ";
    auto now = std::chrono::system_clock::now();
    time_t time_now = std::chrono::system_clock::to_time_t(now);
    myfile << "HORA: " << time_now << "\n";
    myfile.close();
    sleep(sleep_seconds);
    return 0;
}

// create a connection with coordinator to send and receive messages
void converse(int sockfd, int n_repeat, int sleep_seconds) {
    // create a buffer to read from coordinator
    char buffer[BUFFER_SIZE];
    std::string request, release;
    int n;

    // create and config connection with coordinator (server)
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVERADDR_PORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // repeat n requests for the same process
    while (n_repeat > 0)
    {
        cout << "sending REQUEST to coordinator...\n" << endl;

        // send request to coordinator
        // wait until coordinator send a response
        request = encode_message(MESSAGE_REQUEST, BUFFER_SIZE);
        n = sendto(sockfd, request.c_str(), strlen(request.c_str()), 0,
                   (struct sockaddr *) &serveraddr, sizeof(serveraddr));
        if (n < 0)
            error((char*)"ERROR in sendto");

        // receive the response
        n = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (n < 0)
            error((char*)"ERROR in recv");

        cout << "coordinator response: " << buffer << "\n" << endl;

        // verify response from coordinator
        // if invalid, send another request
        message msg = decode_message(buffer);
        if(msg.valid) {
            cout << msg.message_type << endl;
            if (strcmp(msg.message_type, MESSAGE_GRANT) == 0) {

                cout << "writing in result.txt\n" << endl;

                write_file(sleep_seconds);
                if (n < 0)
                    error((char*)"ERROR writing file");
                n_repeat -= 1;

                cout << "send RELEASE to coordinator\n" << endl;

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
    int n_repeat = atoi(argv[1]);
    int port = atoi(argv[2]);
    int sleep_seconds = atoi(argv[3]);
    int sockfd = connect(port);
    converse(sockfd, n_repeat, sleep_seconds);
    disconnect(sockfd);
}