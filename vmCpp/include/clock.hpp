#ifndef CLOCK_HPP
#define CLOCK_HPP

#include "main.hpp"

class Clock
{
    private:
        int speed;
        uint8 state;

    public:
        Clock(int speed);
        Clock();
        void tick(void);
};

#endif