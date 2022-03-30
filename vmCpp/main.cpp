#include "include/main.hpp"
#include "include/vm.hpp"

int main(int argc, char *argv[])
{
    uint32 ram_size;
    VirtualMachine *vm;
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
    vm = new VirtualMachine(new Ram(ram_size));
    vm->getRam()->setMemoryTo("test.txt");
    //vm->displayRam();
    delete (vm);
}