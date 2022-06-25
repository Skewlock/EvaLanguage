/**
 * @file buses.cpp
 * @author Mathias Couriol (mathias.couriol@gmail.com)
 * @brief Buses to link the CPU and the RAM or other devices
 * @version 0.1
 * @date 2022-04-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../include/buses.hpp"

/**
 * @brief Construct a new Buses:: Buses object
 * 
 */
Buses::Buses()
{
    this->addressBus = 0;
    this->dataBus = 0;
    this->readWritePin = 0;
}

Buses::~Buses()
{
    
}

/**
 * @brief return the data bus 
 * 
 * @return uint64 
 */
uint64 Buses::getDataBus(void)
{
    return (this->dataBus);
}

/**
 * @brief return the address bus
 * 
 * @return uint32 
 */
uint32 Buses::getAddressBus(void)
{
    return (this->addressBus);
}

/**
 * @brief return the Read/Write pin
 * 
 * @return uint8 
 */
uint8 Buses::getReadWritePin(void)
{
    return (this->readWritePin);
}

uint8 Buses::getDataSizeBus(void)
{
    return (this->dataSizeBus);
}

/**
 * @brief Set the data bus
 * 
 * @param data 
 */
void Buses::setDataBus(uint64 data)
{
    this->dataBus = data;
}

/**
 * @brief Set the address bus
 * 
 * @param address 
 */
void Buses::setAddressBus(uint32 address)
{
    this->addressBus = address;
}

/**
 * @brief Set the Read/Write pin
 * 
 * @param pin 
 */
void Buses::setReadWritePin(uint8 pin)
{
    this->readWritePin= pin;
}


/**
 * @brief set the data size bus
 * 
 * @param size 
 */
void Buses::setDataSizeBus(uint8 size)
{
    this->dataSizeBus = size;
}

void Buses::addObserver(Peripheral *p)
{
    this->observerList.push_back(p);
}

void Buses::setBusesAndNotify(uint8 rw, uint32 address, uint8 size)
{
    this->setAddressBus(address);
    this->setReadWritePin(rw);
    this->setDataSizeBus(size);
    this->notifyPeriph();
}

void Buses::setBusesAndNotify(uint8 rw, uint32 address, uint64 data, uint8 size)
{
    this->setAddressBus(address);
    this->setReadWritePin(rw);
    this->setDataBus(data);
    this->setDataSizeBus(size);
    this->notifyPeriph();
}

void Buses::notifyPeriph()
{
    Peripheral *p;
    std::list<Peripheral *>::iterator it = this->observerList.begin();
    for (long unsigned int i = 0; i < this->observerList.size(); i++)
    {
        p = (*it);
        if ((*it)->getBeginAddr() <= this->getAddressBus() && this->getAddressBus() <= (*it)->getEndAddr())
            p->execOrder();
        std::advance(it, 1);
    }
}