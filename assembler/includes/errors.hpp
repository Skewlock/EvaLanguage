#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <string>

class Error
{
    public:
        Error(int l, int c, std::string fn, std::string em);
        ~Error();
        void    display_error(std::string type);

    private:
        std::string error_msg;
        std::string fileName;
        int line;
        int column;

};

class FileError:
public Error
/*
    - Unspecified input
    - Unreadable input
    - Unwritable output
*/
{
    public:
        static const std::string type;
        FileError(int l, int c, std::string fn, std::string em);
        void    display_error(void);
};

class SyntaxError:
public Error
/*
    - Unknown OpCode
    - Unknown Condition
    - Unknown operand
*/
{
    public:
        static const std::string type;
        SyntaxError(int l, int c, std::string fn, std::string em);
        void    display_error(void);
};


class PreprocessorError:
public Error
/*
    -invalid label
*/
{
    public:
        static const std::string type;
        PreprocessorError(int l, int c, std::string fn, std::string em);
        void    display_error(void);
};

/*class SemanticError:
public Error

    - Unreachable Address
    - Infinite loop

{

};*/

#endif