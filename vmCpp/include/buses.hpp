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
        uint64 getDataBus(void);
        uint32 getAddressBus(void);
        uint8 getReadWritePin(void);
        void setDataBus(uint64 data);
        void setAddressBus(uint32 address);
        void setReadWritePin(uint8 pin);
};

#endif