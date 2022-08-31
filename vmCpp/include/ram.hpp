#ifndef RAM_HPP
#define RAM_HPP

#include "general.hpp"
#include "buses.hpp"
#include <fstream>

#include "peripheral.hpp"

class Buses;
class Peripheral;


class Ram: public Peripheral
{
    private:
        Buses *buses;
        uint32 ram_size;
        uint8 *ram;

    public:
        Ram(uint32 size, Buses *b);
        virtual ~Ram();
        uint8 readAddr8(uint32 address);
        void storeAddr8(uint32 address, uint8 data);
        uint32 readAddr32(uint32 address);
        void storeAddr32(uint32 address, uint32 data);
        uint64 readAddr64(uint32 address);
        void storeAddr64(uint32 address, uint64 data);
        void setMemoryTo(std::string fileName);
        uint32 getRamSize(void);
        void updateBuses(void);
        void execOrder(void);
        std::string getName(void);
};

#endif