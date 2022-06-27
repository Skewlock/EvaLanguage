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
#include <sstream>
#include <vector>

std::vector<std::string> split(std::string s, char del = ' ')
{
    std::vector<std::string> res;
    std::string tok;
    std::stringstream stream(s);
    while(std::getline(stream, tok, del))
        res.push_back(tok);
    return res;
}

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
    std::string filename;
    std::vector<std::string> flag;

    // checking correct args size
    switch(argc)
    {
        case 1:
            ram_size = RAM_MIN;
            filename = "memory.mem";
            break;
        case 2:
            flag = split(std::string(argv[1]), '=');
            if (flag.front() == "-ramSize")
            {
                long int temp = atoi(flag.back().c_str());
                if (temp < RAM_MIN || temp > RAM_MAX)
                {
                    std::cerr << "Invalid RAM size." << std::endl;
                    return (-1);
                }
                ram_size = (uint32) temp;
                filename = "memory.mem";
            }
            else if (flag.front() == "-fileName")
            {
                filename = flag.back();
                ram_size = RAM_MIN;
            }
            break;

        case 3:
            flag = split(std::string(argv[1]), '=');
            if (flag.front() == "-ramSize")
            {
                long int temp = atoi(flag.back().c_str());
                if (temp < RAM_MIN || temp > RAM_MAX)
                {
                    std::cerr << "Invalid RAM size." << std::endl;
                    return (-1);
                }
                ram_size = (uint32) temp;
                flag = split(std::string(argv[2]), '=');
                filename = flag.back();
            }
            else if (flag.front() == "-fileName")
            {
                filename = flag.back();
                flag = split(std::string(argv[2]), '=');
                long int temp = atoi(flag.back().c_str());
                if (temp < RAM_MIN || temp > RAM_MAX)
                {
                    std::cerr << "Invalid RAM size." << std::endl;
                    return (-1);
                }
                ram_size = (uint32) temp;
            }
            break;
    };

    //create the VM with the good RAM size
    vm = new VirtualMachine(ram_size);
    vm->getRam()->setMemoryTo("memory.mem");
    vm->run();
    delete (vm);
}