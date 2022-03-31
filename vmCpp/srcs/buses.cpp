#include "../include/buses.hpp"

Buses::Buses()
{
    this->addressBus = 0;
    this->dataBus = 0;
    this->readWritePin = READ;
}

uint64 Buses::getDataBus(void)
{
    return (this->dataBus);
}

uint32 Buses::getAddressBus(void)
{
    return (this->addressBus);
}

uint8 Buses::getReadWritePin(void)
{
    return (this->readWritePin);
}

void Buses::setDataBus(uint64 data)
{
    this->dataBus = data;
}

void Buses::setAddressBus(uint32 address)
{
    this->addressBus = address;
}

void Buses::setReadWritePin(uint8 pin)
{
    this->readWritePin= pin;
}