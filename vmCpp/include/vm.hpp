#ifndef VM_HPP
#define VM_HPP

#include "main.hpp"

class Cpu;
class Ram;
class Buses;

class VirtualMachine
{
    private:
        Cpu *cpu;
        Ram *ram;
        uint64 dataBus;
        uint32 addressBus;
        uint8 readWrite;
    public:
        VirtualMachine(int ram_size);
        ~VirtualMachine();
        void displayRam(void);
        Ram *getRam(void);
        void cpuCycle(void);
        void displayRegisters(void);
        Cpu *getCpu(void);

        // data buses and CPU pins
        uint64 getDataBus(void);
        uint32 getAddressBus(void);
        uint8 getReadWritePin(void);
        void setDataBus(uint64 data);
        void setAddressBus(uint32 address);
        void setReadWritePin(uint8 pin);
};

#endif