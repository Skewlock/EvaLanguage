/**
 * @file position.cpp
 * @author Mathias Couriol (mathias.couriol@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../include/position.hpp"

/**
 * @brief Construct a new Position:: Position object
 * 
 * @param l 
 * @param c 
 * @param ix 
 * @param fn 
 * @param ft 
 */
Position::Position(int l, int c, int ix, std::string fn, std::string ft)
{
    this->line = l;
    this->column = c;
    this->index = ix;
    this->fileName = fn;
    this->fileText = ft;
}

/**
 * @brief advance the position (called when advancing the lexer)
 * 
 * @param curr_char 
 */
void Position::advance(char curr_char)
{
    this->index++;
    this->column++;
    if (curr_char == '\n')
    {
        this->line++;
        this->column = 0;
    }
}

/**
 * @brief copy the position and return it
 * 
 * @return Position* 
 */
Position *Position::copy(void)
{
    return new Position(this->line, this->column, this->index, this->fileName, this->fileText);
}

std::string Position::getFileName(void)
{
    return this->fileName;
}

int Position::getLine(void)
{
    return this->line;
}

int Position::getIndex(void)
{
    return this->index;
}

int Position::getColumn(void)
{
    return this->column;
}