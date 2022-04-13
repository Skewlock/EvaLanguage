#ifndef RAM_HPP
#define RAM_HPP

#include "main.hpp"
#include <fstream>

class Buses;

class Ram
{
    private:
        Buses *buses;
        uint32 ram_size;
        uint8 *ram;

    public:
        Ram(uint32 size, Buses *b);
        ~Ram();
        uint8 readAddr8(uint32 address);
        void storeAddr8(uint32 address, uint8 data);
        uint64 readAddr64(uint32 address);
        void storeAddr64(uint32 address, uint64 data);
        void setMemoryTo(std::string fileName);
        uint32 getRamSize(void);
        void updateBuses(void);
};

#endif