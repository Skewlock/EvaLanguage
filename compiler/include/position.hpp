#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>

class Position
{
    private:
        int line;
        int column;
        int index;
        std::string fileName;
        std::string fileText;
    
    public:
        Position(int l, int c, int ix, std::string fn, std::string ft);
        void advance(char curr_char);
        std::string getFileName(void);
        int getLine(void);
        int getIndex(void);
        int getColumn(void);
        Position *copy(void);
};

#endif