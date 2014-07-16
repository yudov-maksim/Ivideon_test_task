#include <iostream>
#include "Server.h"

using namespace std;

/*
void sig_pipe_hndlr(int sig){
    cout << "Catched";
    exit(0);
}
*/

int main()
{
    /*
    //настройка обработчика сигнала sigpipe
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = sig_pipe_hndlr;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGPIPE);
    sigaddset(&set, SIGINT);
    action.sa_mask = set;
    sigaction(SIGPIPE, &action, 0);
    */

    Server serv((string)"/tmp/ivideon_request_pipe", (string)"/tmp/ivideon_answer_pipe_");
    serv.work();
    return 0;
}

