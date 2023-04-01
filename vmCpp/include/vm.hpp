#ifndef VM_HPP
#define VM_HPP

#include "general.hpp"
#include "gpu.hpp"
#include "cpu.hpp"
#include "ram.hpp"
#include "buses.hpp"

class Cpu;
class Ram;
class Buses;

class VirtualMachine
{
    private:
        Gpu *gpu;
        Cpu *cpu;
        Ram *ram;
        uint64 dataBus;
        uint32 addressBus;
        uint8 readWrite;
    public:
        VirtualMachine(int ram_size);
        ~VirtualMachine();
        Ram *getRam(void);
        Cpu *getCpu(void);
        void run(void);
};

#endif