#include "../include/clock.hpp"

Clock::Clock(int speed)
{
    this->speed = speed;
    this->state = 0;
}

Clock::Clock()
{
    this->speed = 100;
    this->state = 0;
}

void Clock::tick(void)
{
    this->state = (this->state == 0);
}