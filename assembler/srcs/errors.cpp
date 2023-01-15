#include "../includes/errors.hpp"
#include <iostream>

const std::string FileError::type = std::string("FileError");
const std::string SyntaxError::type = std::string("SyntaxError");

Error::Error(int l, int c, std::string fn, std::string em)
{
    this->fileName = fn;
    this->column = c;
    this->line = l;
    this->error_msg = em;
}

Error::~Error()
{

}

void    Error::display_error(std::string type)
{
    std::cerr << type << ": \nFile: " << this->fileName << ", Line: " << this->line
    << ", Column: " << this->column << "\n" << this->error_msg << std::endl;
}

FileError::FileError(int l, int c, std::string fn, std::string em):
Error(l, c, fn, em)
{

}

void    FileError::display_error(void)
{
    Error::display_error(FileError::type);
}

SyntaxError::SyntaxError(int l, int c, std::string fn, std::string em):
Error(l, c, fn, em)
{
    
}

void    SyntaxError::display_error(void)
{
    Error::display_error(SyntaxError::type);
}