/**
 * @file vm.cpp
 * @author Mathias Couriol (mathias.couriol@gmail.com)
 * @brief Basically the main class linking every other classes. The virtual machine in fact
 * @version 0.1
 * @date 2022-04-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../include/vm.hpp"

/**
 * @brief Construct a new Virtual Machine:: Virtual Machine object
 * 
 * @param ram_size 
 */
VirtualMachine::VirtualMachine(int ram_size)
{
    Buses *b = new Buses();
    // buses instance must be the same between CPU and RAM
    this->ram = new Ram(ram_size, b);
    this->cpu = new Cpu(b);
    b->addObserver(this->ram);
}

/**
 * @brief Destroy the Virtual Machine:: Virtual Machine object
 * 
 */
VirtualMachine::~VirtualMachine()
{
    delete this->ram;
    delete this->cpu;
}

/**
 * @brief return the RAM
 * 
 * @return Ram* 
 */
Ram *VirtualMachine::getRam(void)
{
    return (this->ram);
}

Cpu* VirtualMachine::getCpu(void)
{
    return this->cpu;
}

void VirtualMachine::run(void)
{
    this->cpu->run();
}