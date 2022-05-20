#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>
#include <iostream>

#define RAM_MIN 32768
#define RAM_MAX 4294967295

//defining general types
typedef unsigned char uint8;
typedef unsigned int uint32;
typedef struct uint48
{
    unsigned long int bits : 48;
} uint48;
typedef unsigned long int uint64;

#include "buses.hpp"
#include "clock.hpp"
#include "cpu.hpp"
#include "ram.hpp"
#include "vm.hpp"

#endif