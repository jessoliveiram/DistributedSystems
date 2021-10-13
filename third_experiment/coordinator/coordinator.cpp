#include <vector>
#include <thread>
#include <netdb.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "../utils/error.h"
#include "../utils/message.h"
#include "../utils/safe_queue.h"
#include "../utils/process_request.h"

#define PORT         8080
#define BUFFER_SIZE  20

using namespace std;
SafeQueue<ProcessRequest> request_queue;

/*
 * Add a valid request to queue
 */
void queue_request(message msg, sockaddr_in clientaddr){
    cout << "Queue Request: " << msg.pid << endl;
    cout << "TYPE: " << msg.message_type << endl;
    ProcessRequest request = ProcessRequest(msg, clientaddr);
    request_queue.enqueue(request);
    
}

/*
 * Send a GRANT message to next client in queue
 */
void release(message msg, sockaddr_in clientaddr){
    cout << "Release: " << msg.pid << endl;
    ProcessRequest request = request_queue.dequeue();
    cout << "get from queue: " << request.get_msg().pid << endl;
}

void process_message(message msg, struct sockaddr_in clientaddr)
{
    if (strcmp(msg.message_type, MESSAGE_REQUEST)==0)
    {
        queue_request(msg, clientaddr);
    }
    else if(strcmp(msg.message_type, MESSAGE_RELEASE) == 0)
    {
        release(msg, clientaddr);
    }
}

/* 
* UDP socket connection reference: 
* https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpserver.c 
*/ 
void listen_udp()
{
    cout << "Start the UDP Server thread..." << endl;
    int sockfd, optval, n;
    socklen_t clientlen;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serveraddr, clientaddr;
    struct hostent *hostp;
    char *hostaddrp;
    std::string response;

    /* 
     * Creating socket file descriptor
     */ 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sockfd < 0 ) {
        error((char*)"socket creation failed");
    }

    /*
     * setsockopt: Handy debugging trick that lets 
     * us rerun the server immediately after we kill it; 
     * otherwise we have to wait about 20 secs. 
     * Eliminates "ERROR on binding: Address already in use" error. 
     */
    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

    /*
     * build the server's Internet address
     */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)PORT);

    /* 
     * bind: associate the parent socket with a port 
     */
    if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) 
        error((char*)"ERROR on binding");


    /* 
     * main loop: wait for a datagram, then echo it
     */
    clientlen = sizeof(clientaddr);
    while (1) 
    {

        /*
         * recvfrom: receive a UDP datagram from a client
         */
        bzero(buffer, BUFFER_SIZE);
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                (struct sockaddr *) &clientaddr, &clientlen);
        if (n < 0)
            error((char*)"ERROR in recvfrom");

        message msg = decode_message(buffer);
        if(msg.valid){
            /* 
            * gethostbyaddr: determine who sent the datagram
            */
            hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
            if (hostp == NULL)
                error((char*)"ERROR on gethostbyaddr");
            hostaddrp = inet_ntoa(clientaddr.sin_addr);
            if (hostaddrp == NULL)
                error((char*)"ERROR on inet_ntoa\n");
            printf("server received datagram from %s (%s)\n", hostp->h_name, hostaddrp);
            printf("server received %d/%d bytes: %s\n", strlen(buffer), n, buffer);
            process_message(msg, clientaddr);
            
            /* 
            * sendto: echo the input back to the client 
            */     
            // response = encode_message(MESSAGE_GRANT, BUFFER_SIZE);
            // cout <<"GRANT RESPONSE: " <<response << endl;
            // n = sendto(sockfd, response.c_str(), strlen(response.c_str()), 0, (struct sockaddr *) &clientaddr, clientlen);
            // if (n < 0) 
            //     error((char*)"ERROR in sendto");
        }
        else{
            cout << "invalid message" << endl;
        }
    }

}

void show_command_line_interface()
{
    cout << "Start the Command Line Interface thread..." << endl;
}

void do_mutal_exclusion()
{
    cout << "Start the Mutal Exclusion thread..." << endl;
}

int main()
{
    vector<thread> program;
    program.emplace_back(listen_udp);
    program.emplace_back(show_command_line_interface);
    program.emplace_back(do_mutal_exclusion);

    for (auto& program_thread : program)
      program_thread.join();

}
