#ifndef RAM_HPP
#define RAM_HPP

#include "main.hpp"
#include <fstream>

class Ram
{
    private:
        uint32 ram_size;
        uint8 *ram;

    public:
        Ram(uint32 size);
        ~Ram();
        uint8 readAddr8(uint32 address);
        void storeAddr8(uint32 address, uint8 data);
        void setMemoryTo(std::string fileName);
        uint32 getRamSize(void);
};

#endif