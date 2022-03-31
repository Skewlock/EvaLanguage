#include "../include/vm.hpp"

VirtualMachine::VirtualMachine(int ram_size)
{
    Buses *b = new Buses();
    // buses instance must be the same between CPU and RAM
    this->ram = new Ram(ram_size, b);
    this->cpu = new Cpu(b);
}

VirtualMachine::~VirtualMachine()
{
    delete this->ram;
    delete this->cpu;
}

Ram *VirtualMachine::getRam(void)
{
    return (this->ram);
}

void VirtualMachine::displayRam()
{
    uint32 size = this->ram->getRamSize();
    for (uint32 i = 0; i < size; i++)
    {
        printf("%02x ", this->ram->readAddr8(i));
        if (i % 16 == 0)
            printf("\n");
    }
}