/**
 * @file token.cpp
 * @author Mathias Couriol (mathias.couriol@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../include/token.hpp"
#include <string>

/**
 * @brief Construct a new Token:: Token object
 * 
 * @param t 
 */
Token::Token(token_types t)
{
    this->type = t;
}

int Token::getType()
{
    return this->type;
}

/**
 * @brief return a string describing the token like : Token{type}
 * 
 * @return std::string 
 */
std::string Token::toString()
{
    return "Token {" + std::to_string(this->type) + "}";
}

/**
 * @brief Construct a new Float Token:: Float Token object
 * 
 * @param t 
 * @param v 
 */
FloatToken::FloatToken(token_types t, float v)
: Token::Token(t)
{
    this->value = v;
}

float FloatToken::getValue()
{
    return this->value;
}

void FloatToken::setValue(float v)
{
    this->value = v;
}

std::string FloatToken::toString()
{
    return "Token {" + std::to_string(this->type) + ", " + std::to_string(this->value) + "}";
}

/**
 * @brief Construct a new Int Token:: Int Token object
 * 
 * @param t 
 * @param v 
 */
IntToken::IntToken(token_types t, int v)
: Token::Token(t)
{
    this->value = v;
}

int IntToken::getValue(void)
{
    return this->value;
}

void IntToken::setValue(int v)
{
    this->value = v;
}

std::string IntToken::toString()
{
    return "Token {" + std::to_string(this->type) + ", " + std::to_string(this->value) + "}";
}

/**
 * @brief Construct a new Op Token:: Op Token object
 * 
 * @param t 
 * @param v 
 */
OpToken::OpToken(token_types t, operation_types v)
: Token::Token(t)
{
    this->value = v;
}

operation_types OpToken::getValue()
{
    return this->value;
}

void OpToken::setValue(operation_types v)
{
    this->value = v;
}

std::string OpToken::toString()
{
    return "Token {" + std::to_string(this->type) + ", " + std::to_string(this->value) + "}";
}