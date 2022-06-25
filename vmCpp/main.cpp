/**
 * @file main.cpp
 * @author Mathias Couriol (mathias.couriol@gmail.com)
 * @brief The main to run the vm
 * @version 0.1
 * @date 2022-04-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "include/main.hpp"

/**
 * @brief the main to run
 * 
 * @param argc 
 * @param argv args must be [program_name, ram_size]
 * @return int 
 */
int main(int argc, char *argv[])
{
    uint32 ram_size;
    VirtualMachine *vm;
    bool running = true;

    // checking correct args size
    if (argc < 2)
        ram_size = RAM_MIN;
    else 
    {
        long int temp = atoi(argv[1]);
        if (temp < RAM_MIN || temp > RAM_MAX)
        {
            std::cerr << "Invalid RAM size." << std::endl;
            return (-1);
        }
        ram_size = (uint32) temp;
    }

    //create the VM with the good RAM size
    vm = new VirtualMachine(ram_size);
    vm->getRam()->setMemoryTo("memory.mem");
    //vm->displayRam();
    int count = 0;
    vm->displayRegisters();
    while (count < 3)
    {
        printf("Cycle %d\n\n", count);
        vm->cpuCycle();
        vm->displayRegisters();
        count++;
    }
    delete (vm);
}