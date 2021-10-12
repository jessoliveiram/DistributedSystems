#include "message.h"


using namespace std;


// encode message to expect format 'message|pid|000'
std::string encode_message(const char* message_number, int len_buffer) {
    std::string message = message_number;
    message += SEPARATOR;
    message += std::to_string(getpid());
    message += SEPARATOR;

    while (message.size() < len_buffer) {
        message += "0";
    }
    return message;
}

// decode format message 'message|pid|000' to get message from process
char decode_message(char message) { return 'a';}
