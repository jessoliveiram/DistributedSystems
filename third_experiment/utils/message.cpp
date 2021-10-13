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

const char* get_message_type(std::string type){   
    if(type == "1")
        return MESSAGE_REQUEST;  
    if(type == "2")
        return MESSAGE_GRANT;
    if(type == "3")
        return MESSAGE_RELEASE;
    return MESSAGE_ERROR;
}

// decode format message 'message|pid|000' to get message from process
message decode_message(std::string msgstr) { 
    message msg;
    size_t pos = msgstr.find(SEPARATOR);
    if(pos == std::string::npos){
        msg.valid = false;
    }
    else{
        msg.message_type = get_message_type(msgstr.substr(0, pos));
        if(msg.message_type ==  MESSAGE_ERROR){
            cout << "erro: " << msg.message_type << endl;
            msg.valid = false;
            return msg; 
        }
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