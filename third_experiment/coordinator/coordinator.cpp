#include <map>
#include <mutex>
#include <vector>
#include <thread>
#include <netdb.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "../utils/error.h"
#include "../utils/message.h"
#include <condition_variable>
#include "../utils/safe_queue.h"
#include "../utils/process_request.h"

#define PORT         8080
#define BUFFER_SIZE  20

using namespace std;
map<std::string, int> process_counter;
SafeQueue<ProcessRequest> request_queue;
SafeQueue<std::string> log_queue;
int sockfd;
std::mutex m;
std::condition_variable c;

/*
 * Add a valid request to queue
 */
void queue_request(message msg, sockaddr_in clientaddr)
{
    ProcessRequest request = ProcessRequest(msg, clientaddr);
    request_queue.enqueue(request);    
}

std::string format_log_text(std::string timestamp, std::string message_type, std::string pid)
{
    std::string log_text = timestamp;
    log_text += SEPARATOR;
    log_text += message_type;
    log_text += SEPARATOR;
    log_text += pid;
    return log_text;
}

/*
 * Read logs from the queue and write in log file
 */
void write_logs() 
{
    std::ofstream log_file;
    while (1)
    {         
        std::string log_msg = log_queue.dequeue();
        //cout << log_msg << endl;
        log_file.open("log.txt", std::ios_base::app);
        log_file << log_msg << "\n";
        log_file.close();
    }
    
}

/*
 * Notify mutex thread to get next process
 */
void release()
{
    c.notify_one();
}

/*
 * Verify message_type and call the correspondent function to precess it.
 */
void process_message(message msg, struct sockaddr_in clientaddr)
{
    auto now = std::chrono::system_clock::now();
    time_t time_now = std::chrono::system_clock::to_time_t(now);
        
    if (strcmp(msg.message_type, MESSAGE_REQUEST)==0)
    {
        log_queue.enqueue(format_log_text(to_string(time_now),"REQUEST", msg.pid));
        queue_request(msg, clientaddr);
    }
    else if(strcmp(msg.message_type, MESSAGE_RELEASE) == 0)
    {
        log_queue.enqueue(format_log_text(to_string(time_now),"RELEASE", msg.pid));
        release();
    }
}

/* 
* UDP socket connection reference: 
* https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpserver.c 
*/ 
void listen_udp()
{
    cout << "Start the UDP Server thread... \n" << endl;
    int optval, n;
    socklen_t clientlen;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serveraddr, clientaddr;
    struct hostent *hostp;
    char *hostaddrp;
    

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
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &clientaddr, &clientlen);
        if (n < 0)
            error((char*)"ERROR in recvfrom");

        message msg = decode_message(buffer);
        if(msg.valid){
            /* 
            * gethostbyaddr: determine who sent the datagram
            */
            process_message(msg, clientaddr);
            
        }
        else{
            cout << "invalid message" << endl;
        }
    }

}

enum command
{
    help,
    cprint_queue,
    print_process_history,
    cexit,
};

command parse_command(std::string command)
{
    if((command == "queue") || (command == "1"))
        return cprint_queue;
    else if ((command == "history") || (command == "2"))
        return print_process_history;
    else if ((command == "exit") || (command == "3"))
        return cexit;
    else
        return help;
       
}

void print_queue()
{
    int count = 1;
    std::queue<ProcessRequest> copied_queue = request_queue.copy_queue();
    cout << "\nPROCESS IN QUEUE: \n";
    cout << "#\tPID\n";

    while (!copied_queue.empty())
    {
        ProcessRequest enqueued_request = copied_queue.front();
        copied_queue.pop();
        cout << count <<"\t" << enqueued_request.get_msg().pid << '\n';
        count +=1;
    }
    cout << endl;
}

void print_process_counter()
{
    map<std::string, int>::iterator itr;
    cout << "\nPROCESS EXECUTED: \n";
    cout << "PID\t\tCount\n";
    for (itr = process_counter.begin(); itr != process_counter.end(); ++itr) 
    {
        cout << itr->first << '\t'<<'\t' << " "<< itr->second << '\n';
    }
    cout << endl;
}

void show_command_line_interface()
{
    cout << "Start the Command Line Interface thread...\n" << endl;
    std::string command;
    while (1)
    {
        getline(cin, command);
        switch (parse_command(command))
        {
        case help:
            cout << "---- HELP -----" << endl;
            cout << "'1' or 'queue': print all requests in queue" << endl;
            cout << "'2' or 'history': print how many times each process has been allowed" << endl;
            cout << "'3' or 'exit': quit the program" << endl;
            
            break;
        case cprint_queue:
            print_queue();
            break;
        case print_process_history:
            print_process_counter();
            break;
        case cexit:
            kill(getpid(), 9);      
        }
    }
    
}

/*
 * Send a GRANT to the first element of process_queue or wait if the queue is empty
 * Wait RELEASE to consume the next element
 */
void do_mutal_exclusion()
{
    int n;
    cout << "Start the MUTEX thread... \n" << endl;
    while (1)
    {   
        ProcessRequest request = request_queue.dequeue();
        struct sockaddr_in clientaddr = request.get_clientaddr();
        std::string response = encode_message(MESSAGE_GRANT, BUFFER_SIZE);
        n = sendto(sockfd, response.c_str(), strlen(response.c_str()), 0, (struct sockaddr *) &clientaddr, sizeof(clientaddr));
        auto now = std::chrono::system_clock::now();
        time_t time_now = std::chrono::system_clock::to_time_t(now);
        log_queue.enqueue(format_log_text(to_string(time_now),"GRANT", request.get_msg().pid));
        if (n < 0) 
          error((char*)"ERROR in sendto");

        map<std::string, int>::iterator itmap = process_counter.find(request.get_msg().pid);

        if (itmap == process_counter.end())
        {
            process_counter.insert(pair<std::string, int>(request.get_msg().pid, 1));
        }else{

            itmap->second +=1; 
        }
        
        std::unique_lock<std::mutex> lock(m);
        c.wait(lock);
    }
    
}

int main()
{
    vector<thread> program;
    program.emplace_back(listen_udp);
    program.emplace_back(do_mutal_exclusion);
    program.emplace_back(show_command_line_interface);
    program.emplace_back(write_logs);

    for (auto& program_thread : program)
      program_thread.join();

}
