#include "Server.h"

using namespace std;

Server::Server(string request_pipe_name, string answer_pipe_main_name){
    this->request_pipe_name = request_pipe_name;
    this->answer_pipe_main_name = answer_pipe_main_name;
    buf_size = 512-1;//по умолчанию
}

int Server::create_pipe(string pipe_name, string pid){
    if (pid != ""){
        pipe_name += pid;
    }
    if (access(pipe_name.c_str(), F_OK))
        if (mkfifo(pipe_name.c_str(), 0777) == -1){
            cout << "Error in create pipe" << endl;
            return -1;
        }
    return 0;
}

void Server::work(){
    char *request_buf = new char [buf_size];
    int res_op=0;
    string str_request;
    string str_answer;

    if (create_pipe(request_pipe_name)){
        return;
    }
    request_descriptor = open(request_pipe_name.c_str(), O_RDONLY);
    if (request_descriptor == -1){
        cout << "Error in open request pipe" << endl;
        return;
    }

    while (1){

        memset(request_buf, 0, buf_size);

        //прочитать запрос
        res_op = read(request_descriptor, request_buf, buf_size);
        if (!res_op){
            close(request_descriptor);
            request_descriptor = open(request_pipe_name.c_str(), O_RDONLY);
            continue;
            if (request_descriptor == -1){
                cout << "Error in open request pipe" << endl;
                break;
            }
        }else if (res_op == -1){
            cout << "Error in read request pipe" << endl;
            break;
        }

        str_request = (const char *) request_buf;
        //cout << "-------Получен запрос: " << str_request;
        //обработать запрос, вытащить оттуда pid
        int num_position;
        string pid;
        num_position = str_request.find('_', 0);
        pid = str_request.substr(0, num_position);
        str_request.erase(0, num_position+1);
        cout << "pid: " << pid << " request: "<<str_request;
        //разбить запрос на слова
        vector <string> *vect_request = build_request(str_request);
        //выполнить запрос, создать ответ
        if (vect_request != NULL){
            str_answer = run_request(vect_request);
        }else{
            str_answer = "failed";
        }
        vect_request->clear();
        delete vect_request;
        //отправить ответ в нужный канал
        int answer_descriptor = open((answer_pipe_main_name + pid).c_str(), O_WRONLY|O_NDELAY);
        int it=0;
        while (answer_descriptor == -1){
            //cout << "Error in open pipe for " << pid << endl;
            usleep(1000);
            answer_descriptor = open((answer_pipe_main_name + pid).c_str(), O_WRONLY|O_NDELAY);
            //return;
            if (++it>100){
                //считаем, что канал сломан
                break;
            }
        }

        if (answer_descriptor != -1){
            write(answer_descriptor, str_answer.c_str(), sizeof(char)*str_answer.length());
            close(answer_descriptor);
        }
    }

    //очистка выделенных ресурсов
    delete[] request_buf;
    if (request_descriptor != -1){
        close(request_descriptor);
    }
    unlink(request_pipe_name.c_str());
}

//запрос -  command [arg]
vector <string>* Server::build_request(string request){
    if (request == ""){
        return NULL;
    }
    request.erase(request.length()-1, 1);
    int num_delim_pos = request.find(' ', 0);
    vector <string> *res = new vector<string>();

    //разобьем запрос
    if (num_delim_pos == -1){
        //без аргументов
        res->push_back(request);
    } else{
        res->push_back(request.substr(0, num_delim_pos));
        if (request.length() > num_delim_pos+1){
            res->push_back(request.substr(num_delim_pos+1, request.length()));
        }
    }
    return res;
}

string Server::run_request(vector<string> *request){
    string res="failed";
    if (request->size() == 1){
        if (request->at(0) == "get_led_state"){
            res = "ok " + led.get_led_state();
        }
        else if (request->at(0) == "get_led_color"){
            res = "ok " + led.get_led_color();
        }else if (request->at(0) == "get_led_rate"){
            res = "ok " + led.get_led_rate();
        }
    }else if (request->size() == 2){
        if (request->at(0) == "set_led_state"){
            res = "ok";
            if (request->at(1) == "on"){
                led.set_led_state(on);
            }else if (request->at(1) == "of"){
                led.set_led_state(of);
            }else{
                res = "failed";
            }
        }
        else if (request->at(0) == "set_led_color"){
            res = "ok ";
            if (request->at(1) == "red"){
                led.set_led_color(red);
            }else if (request->at(1) == "green"){
                led.set_led_color(green);
            }else if (request->at(1) == "blue"){
                led.set_led_color(blue);
            }else{
                res = "failed";
            }
        }else if (request->at(0) == "set_led_rate"){
            if (!led.set_led_rate(atoi((request->at(1)).c_str()))){
                res = "ok";
            }else{
                res="failed";
            }

        }
    }
    return res;
}


