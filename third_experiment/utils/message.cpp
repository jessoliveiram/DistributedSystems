#include "message.h"

// get pid of current process
int get_pid() {
    int pid = fork();
    if (pid == 0)
        return getpid();
    return -1;
}

// encode message to expect format 'message|pid|000'
using namespace std;
char encode_message(char message_number, char pid, char len_buffer) {
    char msg[len_buffer];
    msg[0] = message_number;
    msg[1] = SEPARATOR;
    strcat(msg, pid);
    strcat(msg, SEPARATOR);
    while (strlen(msg) < len_buffer) {
        strcat(msg, '0');
    }
    return msg;
}

// decode format message 'message|pid|000' to get message from process
string decode_message(string message) {}
