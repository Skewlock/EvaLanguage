#include "../include/peripheral.hpp"

Peripheral::Peripheral(int begin, int end)
{
    this->beginning_address = begin;
    this->end_address = end;
}

std::string Peripheral::getName()
{
    return "Peripheral";
}

uint32 Peripheral::getBeginAddr()
{
    return this->beginning_address;
}

uint32 Peripheral::getEndAddr()
{
    return this->end_address;
}