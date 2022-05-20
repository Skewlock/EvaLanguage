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

/**
 * @brief Construct a new Cpu:: Cpu object
 * 
 * @param b 
 */
Cpu::Cpu(Buses *b)
{
    this->buses = b;
    this->regG = new uint64[R_GCOUNT];
    this->regG[R_PC] = 0x1000;
    this->regS = new uint48[R_SCOUNT];
}

/**
 * @brief Destroy the Cpu:: Cpu object
 * 
 */
Cpu::~Cpu()
{
    delete this->regG;
    delete this->regS;
}

/**
 * @brief return the CPU general registers
 * 
 * @return uint64* 
 */
uint64 *Cpu::getGRegisters(void)
{
    return (this->regG);
}

/**
 * @brief Set the CPU designed general register
 * 
 * @param r 
 * @param value 
 */
void Cpu::setGRegister(int r, uint64 value)
{
    this->regG[r] = value;
}

/**
 * @brief return the CPU specific register
 * 
 * @return uint48* 
 */
uint48  *Cpu::getSRegisters(void)
{
    return (this->regS);
}

/**
 * @brief Set the CPU designed specific register
 * 
 * @param r 
 * @param value 
 */
void Cpu::setSRegisters(int r, uint48 value)
{
    this->regS[r] = value;
}

/**
 * @brief Update the buses to allow communication with RAM or other devices
 * 
 */
void Cpu::updateBuses(uint8 rw, uint32 addr, uint64 data)
{
    // :(
}

/**
 * @brief Get the next OP from RAM
 * 
 */
void Cpu::fetchNextOp1(void)
{
    this->buses->setAddressBus(this->regG[R_PC]);
    this->buses->setReadWritePin(READ);
}

void Cpu::fetchNextOp2(void)
{
    uint64 instruction = (this->buses->getDataBus() & 0xFFFFFFFFFFFF0000) >> 16;
    this->regS[R_IR] = *(uint48 *) &instruction;
    this->regG[R_PC] += 6;
}

/**
 * @brief Decode the next OP
 * 
 * @return uint8* 
 */
void Cpu::decodeOp(void)
{
    uint48 inst = this->regS[R_IR];
    this->inst_tab[0] = (inst.bits & 0xFC0000000000) >> 42; // family + opcode
    this->inst_tab[1] = (inst.bits & 0x020000000000) >> 41; // S bit
    this->inst_tab[2] = (inst.bits & 0x010000000000) >> 40; // C bit
    this->inst_tab[3] = (inst.bits & 0x00F000000000) >> 36; // dest
    this->inst_tab[4] = (inst.bits & 0x000F00000000) >> 32; // 1st OP
    this->inst_tab[5] = (inst.bits & 0x0000F0000000) >> 28; // 2nd OP
    this->inst_tab[6] = inst.bits & 0x00000FFFFFFF; // data/offset
    //printf("CPU: %lx\n", inst.bits & 0xFC0000000000 >> 42);
    /*for (int i = 0; i < 7; i++)
    {
        printf("inst[%d]: %lx\n", i, inst_tab[i]);
    }*/
}

/**
 * @brief Execute the decoded OP
 *  
 */
void Cpu::executeOp()
{
    //printf("CPU: %lx\n", inst_tab[0]);
    switch (this->inst_tab[0])
    {
        case OP_NOP:
            break;

        case OP_JMP:
            this->regG[R_PC] += this->inst_tab[6] * 6;
            break;
        
        case OP_BRL:
            this->regG[R_LR] = this->regG[R_PC] + 6;
            this->regG[R_PC] += this->inst_tab[6] * 6;
            break;
        
        case OP_BRX:
            this-> regG[R_PC] = this->regG[R_LR];
            break;
        
        case OP_ADD:
            // no flag update
            if (!this->inst_tab[1])
            {
                if (!this->inst_tab[2])
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] + this->regG[this->inst_tab[5]];
                else
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] + this->inst_tab[6];
            }
            else
            {
                // flag C
                if (this->regG[this->inst_tab[5]] > (0xFFFFFFFFFFFFFFFF - this->regG[this->inst_tab[4]]))
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000002;

                if (!this->inst_tab[2])
                {
                    // flag V
                    if (((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 1 
                    && (this->regG[this->inst_tab[4]] & 0x8000000000000000 >> 63) == 0
                    && (this->regG[this->inst_tab[5]] & 0x8000000000000000 >> 63) == 0)
                    || ((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 0 
                    && (this->regG[this->inst_tab[4]] & 0x8000000000000000 >> 63) == 1
                    && (this->regG[this->inst_tab[5]] & 0x8000000000000000 >> 63) == 1))
                        this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000001;
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] + this->regG[this->inst_tab[5]];
                }
                else
                {
                    // flag V
                    if (((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 1 
                    && (this->regG[this->inst_tab[4]] & 0x8000000000000000 >> 63) == 0
                    && (this->inst_tab[6] & 0x8000000 >> 27) == 0)
                    || ((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 0 
                    && (this->regG[this->inst_tab[4]] & 0x8000000000000000 >> 63) == 1
                    && (this->inst_tab[6] & 0x8000000 >> 27) == 1))
                        this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000001;
                }
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] + this->inst_tab[6];

                // flag Z
                if (this->regG[this->inst_tab[3]] == 0)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000004;
                
                // flag N
                if ((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 1)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000008;
            }
            break;

        case OP_SUB:
            uint64 op;
            if (!this->inst_tab[2])
                op = this->regG[this->inst_tab[5]];
            else
                op = this->inst_tab[6];
            op = ~op + 1;
            // no flag update
            if (!this->inst_tab[1])
            {
                if (!this->inst_tab[2])
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] + op;
                else
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] + op;
            }
            else{
                // flag C
                if (this->regG[this->inst_tab[5]] > (0xFFFFFFFFFFFFFFFF - this->regG[this->inst_tab[4]]))
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000002;

                if (!this->inst_tab[2])
                {
                    // flag V
                    if (((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 1 
                    && (this->regG[this->inst_tab[4]] & 0x8000000000000000 >> 63) == 0
                    && (op & 0x8000000000000000 >> 63) == 0)
                    || ((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 0 
                    && (this->regG[this->inst_tab[4]] & 0x8000000000000000 >> 63) == 1
                    && (op & 0x8000000000000000 >> 63) == 1))
                        this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000001;
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] + op;
                }
                else
                {
                    // flag V
                    if (((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 1 
                    && (this->regG[this->inst_tab[4]] & 0x8000000000000000 >> 63) == 0
                    && (op & 0x8000000 >> 27) == 0)
                    || ((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 0 
                    && (this->regG[this->inst_tab[4]] & 0x8000000000000000 >> 63) == 1
                    && (op & 0x8000000 >> 27) == 1))
                        this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000001;
                }
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] + op;

                // flag Z
                if (this->regG[this->inst_tab[3]] == 0)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000004;
                
                // flag N
                if ((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 1)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000008;
            }
            break;
        
        case OP_MUL:
            if (!this->inst_tab[1])
            {
                if (!this->inst_tab[2])
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] * this->regG[this->inst_tab[5]];
                else
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] * this->inst_tab[6];
            }
            break;
        
        case OP_MOV:
            if (!this->inst_tab[2])
                this->regG[inst_tab[3]] = this->regG[inst_tab[5]];
            else
                this->regG[inst_tab[3]] = inst_tab[6];
            break;
    }
}

void Cpu::cycle(void)
{
    this->decodeOp();
    this->executeOp();
}