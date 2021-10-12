#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

#define MESSAGE_REQUEST '1'
#define MESSAGE_GRANT '2'
#define MESSAGE_RELEASE '3'
#define SEPARATOR '|'

int get_pid();

char encode_message(char message_number, char pid, char len_buffer);

string decode_message(string message);