#ifndef BUSES_HPP
#define BUSES_HPP

#include "general.hpp"
#include "peripheral.hpp"

// Buses are here to allow communication between CPU and RAM

enum
{
    READ = 0,
    WRITE = 1
};

class Buses
{
    private:
        uint64 dataBus;
        uint32 addressBus;
        uint8 readWritePin;
        uint8 dataSizeBus;
        std::list<Peripheral *> observerList;
    
    public:
        Buses();
        ~Buses();
        uint64 getDataBus(void);
        uint32 getAddressBus(void);
        uint8 getReadWritePin(void);
        uint8 getDataSizeBus(void);
        void setDataBus(uint64 data);
        void setAddressBus(uint32 address);
        void setReadWritePin(uint8 pin);
        void setDataSizeBus(uint8 size);
        void addObserver(Peripheral *p);
        void setBusesAndNotify(uint8 rw, uint32 addr, uint8 size);
        void setBusesAndNotify(uint8 rw, uint32 addr, uint64 data, uint8 size);
        void notifyPeriph(void);
};

#endif