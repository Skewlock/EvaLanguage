#include "../include/ram.hpp"

Ram::Ram(uint32 size)
{
    this->ram_size = size;
    this->ram = new uint8[this->ram_size];
}

Ram::~Ram()
{
    delete (ram);
}

uint8 Ram::readAddr8(uint32 address)
{
    return (this->ram[address]);
}

void Ram::storeAddr8(uint32 address, uint8 data)
{
    this->ram[address] = data;
}

uint32 Ram::getRamSize(void)
{
    return (this->ram_size);
}

void Ram::setMemoryTo(std::string fileName)
{
    unsigned char temp = 0;
    uint32 i = 0;
    std::ifstream file;
    file.open(fileName);
    while (file)
    {
        temp = file.get();
        this->storeAddr8(i, temp);
        i++;
    }
    file.close();
}