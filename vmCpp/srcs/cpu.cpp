#include "../include/cpu.hpp"
#include "../include/buses.hpp"

Cpu::Cpu(Buses *b)
{
    this->buses = b;
    this->reg = new uint64[15];
}

Cpu::~Cpu()
{
    delete this->reg;
}

uint64 *Cpu::getRegisters(void)
{
    return (this->reg);
}

void Cpu::setRegister(int r, uint64 value)
{
    this->reg[r] = value;
}

void Cpu::updateBuses(void)
{
    this->buses->setAddressBus(this->reg[R_PC]);
    this->buses->setReadWritePin(READ);
}

void Cpu::fetchNextOp(void)
{
    this->reg[R_IR] = this->buses->getDataBus();
}

uint8 *Cpu::decodeOp(void)
{
    uint64 inst = this->reg[R_IR];
    
}