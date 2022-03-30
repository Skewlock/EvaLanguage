#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "main.hpp"

enum {
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_R8,
    R_R9,
    R_PC,   // program counter
    R_IR,   // Instruction Register
    R_SP,   // Stack Pointer
    R_LR,   // Link register
    R_SR,   // Status register
    R_COUNT
};

class Cpu
{
    private:
        uint64 *reg;

    public:
        Cpu();
        uint64 *getRegisters(void);
        void init(void);
};

#endif