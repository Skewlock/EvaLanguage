/**
 * @file cpu.cpp
 * @author Mathias Couriol (mathias.couriol@gmail.com)
 * @brief The CPU
 * @version 0.1
 * @date 2022-04-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../include/cpu.hpp"
#include "../include/buses.hpp"

/**
 * @brief Construct a new Cpu:: Cpu object
 * 
 * @param b 
 */
Cpu::Cpu(Buses *b)
{
    this->buses = b;
    this->reg = new uint64[15];
}

/**
 * @brief Destroy the Cpu:: Cpu object
 * 
 */
Cpu::~Cpu()
{
    delete this->reg;
}

/**
 * @brief return the CPU registers
 * 
 * @return uint64* 
 */
uint64 *Cpu::getRegisters(void)
{
    return (this->reg);
}

/**
 * @brief Set the CPU designed register
 * 
 * @param r 
 * @param value 
 */
void Cpu::setRegister(int r, uint64 value)
{
    this->reg[r] = value;
}

/**
 * @brief Update the buses to allow communication with RAM or other devices
 * 
 */
void Cpu::updateBuses(void)
{
    this->buses->setAddressBus(this->reg[R_PC]);
    this->buses->setReadWritePin(READ);
}

/**
 * @brief Get the next OP from RAM
 * 
 */
void Cpu::fetchNextOp(void)
{
    this->reg[R_IR] = this->buses->getDataBus();
}

/**
 * @brief Decode the next OP
 * 
 * @return uint8* 
 */
uint8 *Cpu::decodeOp(void)
{
    uint64 inst = this->reg[R_IR];
    
}