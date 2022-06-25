#ifndef PERIPHERAL_HPP
#define PERIPHERAL_HPP

#include "general.hpp"

class Peripheral
{
    protected:
        uint32 beginning_address;
        uint32 end_address;
    
    public:
        Peripheral(int begin, int end);
        virtual void execOrder() = 0;
        virtual std::string getName();
        uint32 getBeginAddr(void);
        uint32 getEndAddr(void);
};

#endif