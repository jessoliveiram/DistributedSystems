#include "message.h"


using namespace std;


// encode message to expect format 'message|pid|000'
std::string encode_message(const char* message_type, int len_buffer) {
    std::string msg = message_type;
    msg += SEPARATOR;
    msg += std::to_string(getpid());
    msg += SEPARATOR;

    while (msg.size() < len_buffer) {
        msg += "0";
    }
    return msg;
}

// decode format message 'message|pid|000' to get message from process
message decode_message(std::string msgstr) { 
    message msg;
    size_t pos = msgstr.find(SEPARATOR);
    if(pos == std::string::npos){
        msg.valid = false;
    }
    else{
        msg.message_type = msgstr.substr(0, pos).c_str();
        msgstr.erase(0, pos + 1);
        pos = msgstr.find(SEPARATOR);
        if(pos == std::string::npos){
            msg.valid = false;
        }
        else{
            msg.pid = msgstr.substr(0, pos);
            msg.valid = true;
        }
    }
    return msg;
}

