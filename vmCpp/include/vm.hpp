#ifndef VM_HPP
#define VM_HPP

#include "ram.hpp"
#include "main.hpp"

class VirtualMachine
{
    private:
        Ram *ram;
    public:
        VirtualMachine(Ram *r);
        ~VirtualMachine();
        void displayRam(void);
        Ram *getRam(void);
};

#endif