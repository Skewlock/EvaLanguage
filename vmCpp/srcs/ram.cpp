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
#include "../include/buses.hpp"

/**
 * @brief Construct a new Ram:: Ram object
 * 
 * @param size 
 * @param b 
 */
Ram::Ram(uint32 size, Buses *b)
{
    this->ram_size = size;
    this->ram = new uint8[this->ram_size];
    this->buses = b;
}

/**
 * @brief Destroy the Ram:: Ram object
 * 
 */
Ram::~Ram()
{
    delete ram;
    delete buses;
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