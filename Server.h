#ifndef SERVER_H
#define SERVER_H


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <signal.h>
#include "Led.h"

using namespace std;
class Server
{
    string request_pipe_name;
    string answer_pipe_main_name;
    int request_descriptor;
    int buf_size;
    LED led;

    int create_pipe(string pipe_name, string pid = "");
    vector<string> *build_request(string request);
    string run_request(vector<string> *request);
public:
    void work();
    Server(string request_pipe_name, string answer_pipe_main_name);
};

#endif // SERVER_H
