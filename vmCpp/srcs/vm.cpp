#include "../include/vm.hpp"

VirtualMachine::VirtualMachine(Ram *r)
{
    this->ram = r;
}

VirtualMachine::~VirtualMachine()
{
    delete this->ram;
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