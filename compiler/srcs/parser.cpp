/**
 * @file parser.cpp
 * @author Mathias Couriol (mathias.couriol@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <vector>
#include <iostream>
#include "../include/errors.hpp"
#include "../include/parser.hpp"

BinOp::BinOp(Node *l, Node *o, Node *r)
{
    this->leftChild = l;
    this->rightChild = r;
    this->coreNode = o;
}

void BinOp::addLChild(Node *l)
{
    this->leftChild = l;
}

void BinOp::addRChild(Node *r)
{
    this->rightChild = r;
}

Number::Number(Token *n)
{
    this->number = n;
}

Token *Number::getNumber(void)
{
    return this->number;
}

Node *AST::getRoot(void)
{
    return this->root;
}

void AST::setRoot(Node *n)
{
    this->root = n;
}

Parser::Parser(Lexer *l, std::string ft, std::string fn)
{
    this->fileName = fn;
    this->fileText = ft;
    this->lexer = l;
    this->tree = new AST(NULL);
}

Error *Parser::parse(void)
{
    int priority = 0;
    Error *error = NULL;
    Number *temp;
    error = this->lexer->makeTokens();
    if (error != NULL)
        return error;
    std::vector<Token*> tokens = this->lexer->getTokens();
    for (long unsigned int i = 0; i < tokens.size(); i++)
    {
        std::cout << tokens[i]->toString() << ", ";

        if (tokens[i]->getType() == TT_LPAREN)
            priority++;
        if (tokens[i]->getType() == TT_RPAREN)
        {
            priority--;
            if (priority < 0)
                return error = new InvalidCharError(new Position(1, 1, 1, this->fileName, this->fileText), new Position(1, 1, 1, this->fileName, this->fileText), "')'");
        }
        if (tokens[i]->getType() == TT_INT || tokens[i]->getType() == TT_FLOAT)
        {
            temp = new Number(tokens[i]);
        }
        if (tokens[i]->getType() == TT_OPERATION)
        {
            //TODO: ALEEEEEEEEED
            if (this->tree->getRoot() == NULL)
                this->tree->setRoot(new BinOp(temp))
        }
    }
    std::cout << std::endl;
    return (error);
}