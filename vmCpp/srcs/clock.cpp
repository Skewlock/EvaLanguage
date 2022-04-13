/**
 * @file clock.cpp
 * @author Mathias Couriol (mathias.couriol@gmail.com)
 * @brief the clock
 * @version 0.1
 * @date 2022-04-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../include/clock.hpp"

/**
 * @brief Construct a new Clock:: Clock object
 * 
 * @param speed 
 */
Clock::Clock(int speed)
{
    this->speed = speed;
    this->state = 0;
}

/**
 * @brief Construct a new Clock:: Clock object
 * 
 */
Clock::Clock()
{
    this->speed = 100;
    this->state = 0;
}

/**
 * @brief advance the clock (make it tick)
 * 
 */
void Clock::tick(void)
{
    this->state = (this->state == 0);
}