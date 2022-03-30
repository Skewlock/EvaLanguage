#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <string>
#include "position.hpp"

class Error
{
    private:
        Position *posStart;
        Position *posEnd;
        std::string errorName;
        std::string details;

    public:
        Error(Position *p1, Position *p2, std::string e, std::string d);
        std::string toString(void);
};


class InvalidCharError: public Error
{
    public:
        InvalidCharError(Position *p1, Position *p2, std::string d);
};

class InvalidNumberError: public Error
{
    public:
        InvalidNumberError(Position *p1, Position *p2, std::string d);
};

#endif