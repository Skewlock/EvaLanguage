#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <string>

class Error
{
    public:
        Error(int l, int c, std::string fileName);
        ~Error();
        void    display_error(void);

    private:
        std::string error_msg;
        std::string fileName;
        int line;
        int column;

};

class IncorrectOpCode
// Error::Error(int l, int c, std::string fileName)
{
    public:
        IncorrectOpCode(int l, int c, std::string fileName);
        void    display_error(void);
    
    private:
        std::string error_msg;
        std::string fileName;
        int line;
        int column;
};

#endif