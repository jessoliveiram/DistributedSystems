#include "message.h"
#include <arpa/inet.h>


class ProcessRequest
{
    private:
        struct sockaddr_in clientaddr;
        message msg;
    public:
        ProcessRequest(message m, struct sockaddr_in c){
            msg = m;
            clientaddr = c;
        };

        ProcessRequest();

        message  get_msg(){
            return msg;
        };

        struct sockaddr_in get_clientaddr()
        {
            return clientaddr;
        };
        
};
