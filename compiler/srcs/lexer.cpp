/**
 * @file lexer.cpp
 * @author Mathias Couriol (mathias.couriol@gmail.com)
 * @brief Everything that concerns the lexer
 * @version 0.1
 * @date 2022-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../include/lexer.hpp"
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Construct a new Lexer:: Lexer object
 * 
 * @param input 
 * @param fn 
 */
Lexer::Lexer(std::string input, std::string fn)
{
    this->fileText = input;
    this->fileName = fn;
    this->current_char = 0;
    this->position = new Position(0, 0, -1, fn, input);
    this->advance();
}

/**
 * @brief Advance through the input
 * 
 */
void Lexer::advance(void)
{
    this->position->advance(this->current_char);
    if (this->position->getIndex() < (int)this->fileText.length())
        this->current_char = this->fileText[this->position->getIndex()];
    else
        this->current_char = 0;
}

/**
 * @brief create a token's vector
 * 
 * @return Error* 
 */
Error *Lexer::makeTokens(void)
{
    Error *err;
    std::string whitespaces = " \t\n";
    while ((this->current_char != 0))
    {
        if (DIGITS.find(this->current_char) != std::string::npos)
        {
            err = this->makeNumbers();
            if (err != NULL)
                return err;
        }
        else if (this->current_char == '+')
            this->tokens_list.push_back(new OpToken(TT_OPERATION, TT_PLUS)), this->advance();
        else if (this->current_char == '-')
            this->tokens_list.push_back(new OpToken(TT_OPERATION, TT_MINUS)), this->advance();
        else if (this->current_char == '*')
            this->tokens_list.push_back(new OpToken(TT_OPERATION, TT_MUL)), this->advance();
        else if (this->current_char == '/')
            this->tokens_list.push_back(new OpToken(TT_OPERATION, TT_DIV)), this->advance();
        else if (this->current_char == '%')
            this->tokens_list.push_back(new OpToken(TT_OPERATION, TT_MOD)), this->advance();
        else if (this->current_char == '(')
            this->tokens_list.push_back(new Token(TT_LPAREN)), this->advance();
        else if (this->current_char == ')')
            this->tokens_list.push_back(new Token(TT_RPAREN)), this->advance();
        else if ((whitespaces.find(this->current_char) != std::string::npos))
            this->advance();
        else
        {
            Position *posStart = this->position->copy();
            char character = this->current_char;
            std::string charDetails = std::string("'") + character + std::string("'");
            this->advance();
            return new InvalidCharError(posStart, this->position, charDetails);
        }
    }
    return NULL;
}

/**
 * @brief make a number token based on the digits and dots
 * 
 * @return Error* 
 */
Error *Lexer::makeNumbers()
{
    int dotsCount = 0;
    std::string numberStr = "";
    Position *posStart = this->position->copy();
    while ((this->current_char != 0) && (std::string(DIGITS + ".").find(this->current_char) != std::string::npos))
    {
        if (this->current_char == '.')
        {
            dotsCount++;
            numberStr += ".";
        }
        else
            numberStr = numberStr + this->current_char;
        this->advance();
    }
    if (dotsCount > 1)
        return new InvalidNumberError(posStart, this->position, numberStr);
    else if (dotsCount == 1)
        this->tokens_list.push_back(new FloatToken(TT_FLOAT, std::stof(numberStr)));
    else if (dotsCount == 0)
        this->tokens_list.push_back(new IntToken(TT_INT, std::stoi(numberStr)));
    return NULL;
}

/**
 * @brief get tokens
 * 
 * @return std::vector<Token*> 
 */
std::vector<Token*> Lexer::getTokens(void)
{
    return this->tokens_list;
}