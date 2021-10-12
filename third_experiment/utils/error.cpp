#include "error.h"

/* 
* prints a message error in console and exit 
*/ 
using namespace std;
void error(char *msg)
{   
    cout << msg << endl;
    perror(msg);
    exit(EXIT_FAILURE);
}
