#ifndef BUSES_HPP
#define BUSES_HPP

#include "main.hpp"

enum
{
    READ = 0,
    WRITE = 1
};

// Buses are here to allow communication between CPU and RAM
class Buses
{
    private:
        uint64 dataBus;
        uint32 addressBus;
        uint8 readWritePin;
    
    public:
        Buses();
        ~Buses();
        uint64 Buses::getDataBus(void);
        uint32 Buses::getAddressBus(void);
        uint8 Buses::getReadWritePin(void);
        void Buses::setDataBus(uint64 data);
        void Buses::setAddressBus(uint32 address);
        void Buses::setReadWritePin(uint8 pin);
};

#endif