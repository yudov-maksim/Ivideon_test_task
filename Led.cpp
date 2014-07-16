#include "Led.h"
using namespace std;
LED::LED()
{
    state = of;
    color = red;
    rate = 0;
}

void LED::set_led_state(State st){
    state = st;
}

string LED::get_led_state(){
    string res;
    switch (state){
    case on:
        res="on";
        break;
    case of:
        res="of";
        break;
    default:
        res="of";
    }
    return res;
}

void LED::set_led_color(Color col){
    color = col;
}

string LED::get_led_color(){
    string res="red";
    switch (color){
    case red:
        res="red";
        break;
    case green:
        res="green";
        break;
    case blue:
        res="blue";
        break;
    default:
        res="red";
    }
    return res;
}

int LED::set_led_rate(int rate){
    if (rate>5 || rate<0){
        return -1;
    }else{
        this->rate = rate;
        return 0;
    }
}

string LED::get_led_rate(){
    string res;
    std::ostringstream conv;
    conv << rate;
    res = conv.str();
    return res;
}
