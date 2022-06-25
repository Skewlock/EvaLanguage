#ifndef VM_HPP
#define VM_HPP

#include "general.hpp"
#include "cpu.hpp"
#include "ram.hpp"
#include "buses.hpp"

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
        Cpu *getCpu(void);
        void run(void);
};

#endif