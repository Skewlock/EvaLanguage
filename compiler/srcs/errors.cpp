/**
 * @file errors.cpp
 * @author Mathias Couriol (mathias.couriol@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../include/errors.hpp"

/**
 * @brief Construct a new Error:: Error object
 * 
 * @param p1 
 * @param p2 
 * @param e 
 * @param d 
 */
Error::Error(Position *p1, Position *p2, std::string e, std::string d)
{
    this->posStart = p1;
    this->posEnd = p2;
    this->errorName = e;
    this->details = d;
}

/**
 * @brief transform an Error to a string
 * 
 * @return std::string 
 */
std::string Error::toString(void)
{
    return this->errorName + ": " + this->details + "\nFile "
    + this->posStart->getFileName() + ", Line " + std::to_string(this->posStart->getLine() + 1)
    + ", Column " + std::to_string(this->posStart->getColumn());
}

/**
 * @brief Construct a new Invalid Char Error:: Invalid Char Error object
 * 
 * @param p1 
 * @param p2 
 * @param d 
 */
InvalidCharError::InvalidCharError(Position *p1, Position *p2, std::string d)
: Error(p1, p2, "Invalid Character", d)
{

}

/**
 * @brief Construct a new Invalid Number Error:: Invalid Number Error object
 * 
 * @param p1 
 * @param p2 
 * @param d 
 */
InvalidNumberError::InvalidNumberError(Position *p1, Position *p2, std::string d)
: Error(p1, p2, "Invalid Number", d)
{

}