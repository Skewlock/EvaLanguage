#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include "token.hpp"
#include "position.hpp"
#include "errors.hpp"

const std::string DIGITS = "0123456789";

class Lexer
{
    private:
        std::string fileName;
        std::string fileText;
        std::vector<Token*> tokens_list;
        char current_char;
        Position *position;
    
    public:
        Lexer(std::string input, std::string fn);
        void advance(void);
        Error *makeTokens(void);
        Error *makeNumbers(void);
        std::vector<Token*> getTokens(void);
};

#endif