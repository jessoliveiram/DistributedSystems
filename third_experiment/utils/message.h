#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#define MESSAGE_REQUEST "1"
#define MESSAGE_GRANT "2"
#define MESSAGE_RELEASE "3"
#define SEPARATOR "|"

std::string encode_message(const char*  message_number, int len_buffer);

char decode_message(char message);