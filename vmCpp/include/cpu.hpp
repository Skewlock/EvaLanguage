#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "main.hpp"

class Buses;

enum
{
    OP_NOP = 0x00,
    OP_JMP = 0x01,
    OP_BRL = 0x02,
    OP_BRX = 0X03,
    OP_ADD = 0x10,
    OP_SUB = 0x11,
    OP_MUL = 0x12,
    OP_DIV = 0x13,
    OP_AND = 0x14,
    OP_BOR = 0x15,
    OP_XOR = 0x16,
    OP_RSH = 0x17,
    OP_LSH = 0x18,
    OP_MOV = 0x19,
    OP_CMP = 0x1A,
    OP_LDB = 0x20,
    OP_LDH = 0x21,
    OP_LDW = 0x22,
    OP_STB = 0x23,
    OP_STH = 0x24,
    OP_STW = 0x25,
    OP_PSH = 0x26,
    OP_POP = 0x27
};

enum
{
    R_SR = 0,   // Status register
    R_IR,       // Instruction register
    R_SCOUNT
};

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
    R_10,
    R_11,
    R_12,
    R_SP,   // Stack Pointer
    R_LR,   // Link register
    R_PC,   // program counter
    R_GCOUNT // Register count
};

//instruction:

// 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000

class Cpu
{
    private:
        Buses *buses;
        uint64 *regG;
        uint48 *regS;
        void fetchNextOp(void);
        int *decodeOp();
        void executeOp(int *inst_tab);

    public:
        Cpu(Buses *b);
        ~Cpu();
        uint64 *getGRegisters(void);
        void setGRegister(int r, uint64 value);
        uint48 *getSRegisters(void);
        void setSRegisters(int r, uint48 value);
        void init(void);
        void updateBuses(uint8 rw, uint32 addr, uint64 data);
        void processOp(void);
};

#endif