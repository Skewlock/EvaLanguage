/**
 * @file ram.cpp
 * @author Mathias Couriol (mathias.couriol@gmail.com)
 * @brief The RAM and everything related to it
 * @version 0.1
 * @date 2022-04-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../include/ram.hpp"

/**
 * @brief Construct a new Ram:: Ram object
 * 
 * @param size 
 * @param b 
 */
Ram::Ram(uint32 size, Buses *b) : 
Peripheral::Peripheral(0, size)
{
    this->ram_size = size;
    this->ram = new uint8[this->ram_size];
    this->buses = b;
    for (uint32 i = 0; i < this->ram_size; i++)
    {
        this->ram[i] = 0xFF;
    }
}

/**
 * @brief Destroy the Ram:: Ram object
 * 
 */
Ram::~Ram()
{
    delete [] ram;
    delete buses;
}

/**
 * @brief Return the total RAM size
 * 
 * @return uint32 
 */
uint32 Ram::getRamSize(void)
{
    return (this->ram_size);
}

/**
 * @brief Fill the entire memory with a file
 * 
 * @param fileName 
 */
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

/**
 * @brief Read 8 bits from RAM
 * 
 * @param address 
 * @return uint8 
 */
uint8 Ram::readAddr8(uint32 address)
{
    return (this->ram[address]);
}

/**
 * @brief Store 8 bits in RAM
 * 
 * @param address 
 * @param data 
 */
void Ram::storeAddr8(uint32 address, uint8 data)
{
    this->ram[address] = data;
}

/**
 * @brief read a 32 bits int from memory
 * 
 * @param address 
 * @return uint32 
 */
uint32 Ram::readAddr32(uint32 address)
{
    uint32 res = ((uint32)this->ram[address] << 24) | ((uint32)this->ram[address + 1] << 16)
    | ((uint32)this->ram[address + 2] << 8) | ((uint32)this->ram[address + 3]);
    return (res);
}

/**
 * @brief store a 32 bits int in memory
 * 
 * @param address 
 * @param data 
 */
void Ram::storeAddr32(uint32 address, uint32 data)
{
    this->ram[address]      = data & 0xFF000000 >> 24;
    this->ram[address + 1]  = data & 0x00FF0000 >> 16;
    this->ram[address + 2]  = data & 0x0000FF00 >> 8;
    this->ram[address + 3]  = data & 0x000000FF;
}

/**
 * @brief read a 64 bits int from memory
 * 
 * @param address
 * @return uint64
 */
uint64 Ram::readAddr64(uint32 address)
{
    uint64 res = ((uint64)this->ram[address] << 56) | ((uint64)this->ram[address + 1] << 48)
    | ((uint64)this->ram[address + 2] << 40) | ((uint64)this->ram[address + 3] << 32)
    | ((uint64)this->ram[address + 4] << 24) | ((uint64)this->ram[address + 5] << 16)
    | ((uint64)this->ram[address + 6] << 8) | ((uint64)this->ram[address + 7]);

    return res;
}

/**
 * @brief store a 64 bits int in memory
 * 
 * @param address 
 * @param data 
 */
void Ram::storeAddr64(uint32 address, uint64 data)
{
    this->ram[address]     = data & 0xFF00000000000000 >> 56;
    this->ram[address + 1] = data & 0x00FF000000000000 >> 48;
    this->ram[address + 2] = data & 0x0000FF0000000000 >> 40;
    this->ram[address + 3] = data & 0x000000FF00000000 >> 32;
    this->ram[address + 4] = data & 0x00000000FF000000 >> 24;
    this->ram[address + 5] = data & 0x0000000000FF0000 >> 16;
    this->ram[address + 6] = data & 0x000000000000FF00 >> 8 ;
    this->ram[address + 7] = data & 0x00000000000000FF;
}

std::string Ram::getName()
{
    return "RAM, from " + std::to_string(this->beginning_address) + " to " + std::to_string(this->end_address);
}

void Ram::execOrder()
{
    if (this->buses->getReadWritePin() == READ)
    {
        switch(this->buses->getDataSizeBus())
        {
            case 1:
                this->buses->setDataBus(this->readAddr8(this->buses->getAddressBus()));
                break;
            
            case 4:
                this->buses->setDataBus(this->readAddr32(this->buses->getAddressBus()));
                break;

            case 8:
                this->buses->setDataBus(this->readAddr64(this->buses->getAddressBus()));
                break;
        }
    }
    else if (this->buses->getReadWritePin() == WRITE)
    {
        switch(this->buses->getDataSizeBus())
        {
            case 1:
                this->storeAddr8(this->buses->getAddressBus(), this->buses->getDataBus() >> 56);
                break;
            
            case 4:
                this->storeAddr32(this->buses->getAddressBus(), this->buses->getDataBus() >> 32);
                break;
            
            case 8:
                this->storeAddr64(this->buses->getAddressBus(), this->buses->getDataBus());
                break;
        }
    }
}