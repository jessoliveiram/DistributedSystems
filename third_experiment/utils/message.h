#ifndef MESSAGE
#define MESSAGE
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#define MESSAGE_REQUEST "1"
#define MESSAGE_GRANT "2"
#define MESSAGE_RELEASE "3"
#define SEPARATOR "|"
#define MESSAGE_ERROR "4"

struct message
{
    const char * message_type;
    std::string pid;
    std::string rest;
    bool valid;
};

std::string encode_message(const char*  message_type, int len_buffer);

message decode_message(std::string msgstr);

const char* get_message_type(std::string type);
#endif