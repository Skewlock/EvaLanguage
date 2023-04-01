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
    this->running = true;
    this->buses = b;
    this->regG = new uint64[R_GCOUNT];
    for (int i = 0; i < R_GCOUNT; i++)
    {
        this->regG[i] = 0;
    }
    this->regG[R_PC] = 0x1000;
    this->regS = new uint48[R_SCOUNT];
    for (int i = 0; i < R_SCOUNT; i++)
    {
        this->regS[i].bits = 0;
    }
}

/**
 * @brief Destroy the Cpu:: Cpu object
 * 
 */
Cpu::~Cpu()
{
    delete [] this->regG;
    delete [] this->regS;
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
 * @brief Get the next OP from RAM
 * 
 */
void Cpu::fetchNextOp(void)
{
    this->buses->setBusesAndNotify(READ, this->regG[R_PC], 8);
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
}

/**
 * @brief Execute the decoded OP
 *  
 */
void Cpu::executeOp()
{
    bool n, z, c, v;
    bool regs[R_GCOUNT];
    int parse = 0;
    uint64 op = 0;
    uint64 res = 0;
    uint64 data = 0;
    switch (this->inst_tab[0])
    {
        //Program Flow

        case OP_NOP:
            break;

        case OP_JMP:
            n = (this->regS[R_SR].bits & 0x0000000000000008) >> 3;
            z = (this->regS[R_SR].bits & 0x0000000000000004) >> 2;
            c = (this->regS[R_SR].bits & 0x0000000000000002) >> 1;
            v = (this->regS[R_SR].bits & 0x0000000000000001);
            switch (this->inst_tab[3])
            {   
                case CC_EQ:
                    if (z)
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;
                
                case CC_NE:
                    if (!z)
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;
                
                case CC_CS:
                    if (c)
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;
                
                case CC_CC:
                    if (!c)
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;
                
                case CC_MI:
                    if (n)
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;
                
                case CC_PL:
                    if (!n)
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;
                
                case CC_VS:
                    if (v)
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;
                
                case CC_VC:
                    if (!v)
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;
                
                case CC_HI:
                    if (c && !z)
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;
                
                case CC_LS:
                    if (!c && z)
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;
                
                case CC_GE:
                    if (n == v)
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;
                
                case CC_LT:
                    if (n != v)
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;
                
                case CC_GT:
                    if (!z && (n == v))
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;
                
                case CC_LE:
                    if (z || (n != v))
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;
                
                case CC_AL:
                    this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;

            }
            break;
        
        case OP_BRL:
            n = (this->regS[R_SR].bits & 0x0000000000000008) >> 3;
            z = (this->regS[R_SR].bits & 0x0000000000000004) >> 2;
            c = (this->regS[R_SR].bits & 0x0000000000000002) >> 1;
            v = (this->regS[R_SR].bits & 0x0000000000000001);
            switch (this->inst_tab[3])
            {   
                case CC_EQ:
                    if (z)
                    {
                        this->regG[R_LR] = this->regG[R_PC] + 6;
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    }
                    break;
                
                case CC_NE:
                    if (!z)
                    {
                        this->regG[R_LR] = this->regG[R_PC] + 6;
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    }
                    break;
                
                case CC_CS:
                    if (c)
                    {
                        this->regG[R_LR] = this->regG[R_PC] + 6;
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    }
                    break;
                
                case CC_CC:
                    if (!c)
                    {
                        this->regG[R_LR] = this->regG[R_PC] + 6;
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    }
                    break;
                
                case CC_MI:
                    if (n)
                    {
                        this->regG[R_LR] = this->regG[R_PC] + 6;
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    }
                    break;
                
                case CC_PL:
                    if (!n)
                    {
                        this->regG[R_LR] = this->regG[R_PC] + 6;
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    }
                    break;
                
                case CC_VS:
                    if (v)
                    {
                        this->regG[R_LR] = this->regG[R_PC] + 6;
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    }
                    break;
                
                case CC_VC:
                    if (!v)
                    {
                        this->regG[R_LR] = this->regG[R_PC] + 6;
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    }
                    break;
                
                case CC_HI:
                    if (c && !z)
                    {
                        this->regG[R_LR] = this->regG[R_PC] + 6;
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    }
                    break;
                
                case CC_LS:
                    if (!c && z)
                    {
                        this->regG[R_LR] = this->regG[R_PC] + 6;
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    }
                    break;
                
                case CC_GE:
                    if (n == v)
                    {
                        this->regG[R_LR] = this->regG[R_PC] + 6;
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    }
                    break;
                
                case CC_LT:
                    if (n != v)
                    {
                        this->regG[R_LR] = this->regG[R_PC] + 6;
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    }
                    break;
                
                case CC_GT:
                    if (!z && (n == v))
                    {
                        this->regG[R_LR] = this->regG[R_PC] + 6;
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    }
                    break;
                
                case CC_LE:
                    if (z || (n != v))
                    {
                        this->regG[R_LR] = this->regG[R_PC] + 6;
                        this->regG[R_PC] += this->inst_tab[6] * 6;
                    }
                    break;
                
                case CC_AL:
                    this->regG[R_LR] = this->regG[R_PC] + 6;
                    this->regG[R_PC] += this->inst_tab[6] * 6;
                    break;

            }
            break;
        
        case OP_BRX:
            this-> regG[R_PC] = this->regG[R_LR];
            break;
        
        case OP_END:
            this->running = false;
            break;
        

        // Operations
        
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
            else
            {
                if (!this->inst_tab[2])
                {
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] * this->regG[this->inst_tab[5]];
                    // flag V
                    if ((this->regG[this->inst_tab[4]] != 0 && this->regG[this->inst_tab[3]] < this->regG[this->inst_tab[4]])
                    || (this->regG[this->inst_tab[5]] != 0 && this->regG[this->inst_tab[3]] < this->regG[this->inst_tab[5]]))
                        this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000001;
                }
                else
                {
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] * this->inst_tab[6];
                    if ((this->regG[this->inst_tab[4]] != 0 && this->regG[this->inst_tab[3]] < this->regG[this->inst_tab[4]])
                    || (this->inst_tab[6] != 0 && this->regG[this->inst_tab[3]] < (uint64)this->inst_tab[6]))
                        this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000001;
                }
                // flag Z
                if (this->regG[this->inst_tab[3]] == 0)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000004;
                
                // flag N
                if ((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 1)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000008;
            }
            break;
        
        case OP_DIV:
            if (!this->inst_tab[1])
            {
                if (!this->inst_tab[2])
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] / this->regG[this->inst_tab[5]];
                else
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] / this->inst_tab[6];
            }
            else
            {
                //no flags C nor V
                if (!this->inst_tab[2])
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] / this->regG[this->inst_tab[5]];
                else
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] / this->inst_tab[6];

                // flag Z
                if (this->regG[this->inst_tab[3]] == 0)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000004;
                
                // flag N
                if ((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 1)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000008;
            }
            break;

        case OP_AND:
            if (!this->inst_tab[1])
            {
                if (!this->inst_tab[2])
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] & this->regG[this->inst_tab[5]];
                else
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] & (this->inst_tab[6] | 0xFFFFFFFFF0000000);
            }
            else 
            {
                // No flags C nor V
                if (!this->inst_tab[2])
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] & this->regG[this->inst_tab[5]];
                else
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] & (this->inst_tab[6] | 0xFFFFFFFFF0000000);

                // flag Z
                if (this->regG[this->inst_tab[3]] == 0)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000004;
                
                // flag N
                if ((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 1)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000008;
            }
            break;
        
        case OP_BOR:
            if (!this->inst_tab[1])
            {
                if (!this->inst_tab[2])
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] | this->regG[this->inst_tab[5]];
                else
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] | (this->inst_tab[6] | 0xFFFFFFFFF0000000);
            }
            else
            {
                // No flag C nor V
                if (!this->inst_tab[2])
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] | this->regG[this->inst_tab[5]];
                else
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] | (this->inst_tab[6] | 0xFFFFFFFFF0000000);

                // flag Z
                if (this->regG[this->inst_tab[3]] == 0)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000004;
                
                // flag N
                if ((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 1)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000008;
            }
            break;

        case OP_XOR:
            if (!this->inst_tab[1])
            {
                if (!this->inst_tab[2])
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] ^ this->regG[this->inst_tab[5]];
                else
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] ^ (this->inst_tab[6] | 0xFFFFFFFFF0000000);
            }
            else
            {
                // No flag C nor V
                if (!this->inst_tab[2])
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] ^ this->regG[this->inst_tab[5]];
                else
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] ^ (this->inst_tab[6] | 0xFFFFFFFFF0000000);
                
                // flag Z
                if (this->regG[this->inst_tab[3]] == 0)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000004;
                
                // flag N
                if ((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 1)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000008;
            }
            break;
        
        case OP_RSH:
            if (!this->inst_tab[1])
            {
                if (!this->inst_tab[2])
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] >> this->regG[this->inst_tab[5]];
                else
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] >> this->inst_tab[6];
            }
            else
            {
                if (!this->inst_tab[2])
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] >> this->regG[this->inst_tab[5]];
                else
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] >> this->inst_tab[6];

                // flag Z
                if (this->regG[this->inst_tab[3]] == 0)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000004;
                
                // flag N
                if ((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 1)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000008;
            }
            break;
        
        case OP_LSH:
            if (!this->inst_tab[1])
            {
                if (!this->inst_tab[2])
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] << this->regG[this->inst_tab[5]];
                else
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] << this->inst_tab[6];
            }
            else
            {
                if (!this->inst_tab[2])
                {
                    if ((this->regG[this->inst_tab[4]] >> (64 - this->regG[this->inst_tab[5]]) & 0x1) == 1)
                        this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000002;
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] << this->regG[this->inst_tab[5]];
                }
                else
                {
                    if ((this->regG[this->inst_tab[4]] >> (64 - this->inst_tab[6]) & 0x1) == 1)
                        this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000002;
                    this->regG[this->inst_tab[3]] = this->regG[this->inst_tab[4]] << this->inst_tab[6];
                }

                // flag Z
                if (this->regG[this->inst_tab[3]] == 0)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000004;
                
                // flag N
                if ((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 1)
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000008;
            }
            break;

        case OP_MOV:
            if (!this->inst_tab[2])
                this->regG[inst_tab[3]] = this->regG[inst_tab[5]];
            else
                this->regG[inst_tab[3]] = inst_tab[6];
            break;
        
        case OP_CMP:
            if (!this->inst_tab[2])
                op = this->regG[this->inst_tab[5]];
            else
                op = this->inst_tab[6];
            op = ~op + 1;
            // flag C
            if (this->regG[this->inst_tab[5]] > (0xFFFFFFFFFFFFFFFF - this->regG[this->inst_tab[4]]))
                this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000002;

            if (!this->inst_tab[2])
            {
                // flag V
                if (((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 1 && (this->regG[this->inst_tab[4]] & 0x8000000000000000 >> 63) == 0 && (op & 0x8000000000000000 >> 63) == 0) || ((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 0 && (this->regG[this->inst_tab[4]] & 0x8000000000000000 >> 63) == 1 && (op & 0x8000000000000000 >> 63) == 1))
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000001;
                res = this->regG[this->inst_tab[4]] + op;
            }
            else
            {
                // flag V
                if (((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 1 && (this->regG[this->inst_tab[4]] & 0x8000000000000000 >> 63) == 0 && (op & 0x8000000 >> 27) == 0) || ((this->regG[this->inst_tab[3]] & 0x8000000000000000 >> 63) == 0 && (this->regG[this->inst_tab[4]] & 0x8000000000000000 >> 63) == 1 && (op & 0x8000000 >> 27) == 1))
                    this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000001;
            }
            res = this->regG[this->inst_tab[4]] + op;

            // flag Z
            if (res == 0)
                this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000004;

            // flag N
            if ((res & 0x8000000000000000 >> 63) == 1)
                this->regS[R_SR].bits = this->regS[R_SR].bits | 0x000000000008;
            break;


        // Memory

        case OP_LDW:
            this->buses->setBusesAndNotify(READ, this->regG[R_PC] + this->inst_tab[6], 8);
            data = this->buses->getDataBus();
            this->regG[this->inst_tab[3]] = data;
            break;

        case OP_LDH:
            this->buses->setBusesAndNotify(READ, this->regG[R_PC] + this->inst_tab[6], 4);
            data = this->buses->getDataBus();
            this->regG[this->inst_tab[3]] = data;
            break;

        case OP_LDB:
            this->buses->setBusesAndNotify(READ, this->regG[R_PC] + this->inst_tab[6], 1);
            data = this->buses->getDataBus();
            this->regG[this->inst_tab[3]] = data;
            break;

        case OP_STW:
            data = this->regG[this->inst_tab[3]];
            this->buses->setBusesAndNotify(WRITE, this->regG[R_PC] + this->inst_tab[6], data, 8);
            break;

        case OP_STH:
            data = this->regG[this->inst_tab[3]];
            this->buses->setBusesAndNotify(WRITE, this->regG[R_PC] + this->inst_tab[6], data, 4);
            break;

        case OP_STB:
            data = this->regG[this->inst_tab[3]];
            this->buses->setBusesAndNotify(WRITE, this->regG[R_PC] + this->inst_tab[6], data, 1);
            break;

        case OP_PSH:
            for (int i = 0; i < R_GCOUNT; i++)
            {
                regs[i] = false;
            }
            parse = this->inst_tab[6] & 0xFFFF000;
            regs[0] = parse & 0x8000;
            regs[1] = parse & 0x4000;
            regs[2] = parse & 0x2000;
            regs[3] = parse & 0x1000;
            regs[4] = parse & 0x0800;
            regs[5] = parse & 0x0400;
            regs[6] = parse & 0x0200;
            regs[7] = parse & 0x0100;
            regs[8] = parse & 0x0080;
            regs[9] = parse & 0x0040;
            regs[10] = parse & 0x0020;
            regs[11] = parse & 0x0010;
            regs[12] = parse & 0x0008;
            regs[13] = parse & 0x0004;
            regs[14] = parse & 0x0002;
            regs[15] = parse & 0x0001;
            for (int i = 0; i < R_GCOUNT; i++)
            {
                if (regs[i] == 1)
                    this->buses->setBusesAndNotify(WRITE, this->regG[R_SP], this->regG[i], 8);
            }
            break;

        case OP_POP:
            for (int i = 0; i < R_GCOUNT; i++)
            {
                regs[i] = false;
            }
            parse = this->inst_tab[6] & 0xFFFF000;
            regs[0] = parse & 0x8000;
            regs[1] = parse & 0x4000;
            regs[2] = parse & 0x2000;
            regs[3] = parse & 0x1000;
            regs[4] = parse & 0x0800;
            regs[5] = parse & 0x0400;
            regs[6] = parse & 0x0200;
            regs[7] = parse & 0x0100;
            regs[8] = parse & 0x0080;
            regs[9] = parse & 0x0040;
            regs[10] = parse & 0x0020;
            regs[11] = parse & 0x0010;
            regs[12] = parse & 0x0008;
            regs[13] = parse & 0x0004;
            regs[14] = parse & 0x0002;
            regs[15] = parse & 0x0001;
            for (int i = 0; i < R_GCOUNT; i++)
            {
                if (regs[i] == 1)
                {
                    this->buses->setBusesAndNotify(READ, this->regG[R_SP], 8);
                    this->regG[i] = this->buses->getDataBus();
                    this->regG[R_SP] -= 8;
                }
            }
            break;
    }
}

void Cpu::cycle(void)
{
    if (this->running)
    {
        this->fetchNextOp();
        this->decodeOp();
        this->executeOp();
    }
}

void Cpu::displayRegisters(void)
{
    if (this->running)
    {
        uint64 *r = this->getGRegisters();
        for (int i = 0; i < R_GCOUNT; i++)
        {
            printf("RG%d: 0x%lx\n", i, r[i]);
        }
        uint48 *r2 = this->getSRegisters();
        for (int i = 0; i < R_SCOUNT; i++)
        {
            printf("RS%d: 0x%lx\n", i, (uint64)r2[i].bits);
        }
        printf("\n");
    }
}