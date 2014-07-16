#ifndef LED_H
#define LED_H
#include <iostream>
#include <sstream>

enum State {on, of};
enum Color {red, green, blue};

class LED
{
    State state;
    Color color;
    int rate; //0..5
public:
    LED();

    void set_led_state(State st);
    std::string get_led_state();
    void set_led_color(Color col);
    std::string get_led_color();
    int set_led_rate(int rat);
    std::string get_led_rate();
};

#endif // LED_H
