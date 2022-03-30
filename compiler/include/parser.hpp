#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "lexer.hpp"
#include "errors.hpp"
#include "token.hpp"

class Node
{};

class AST
{
    private:
        Node *root;

    public:
        AST(Node *root);
        Node *getRoot(void);
        void setRoot(Node *n);
};

class BinOp: public Node
{
    private:
        Node *leftChild;
        Node *rightChild;
        Node *coreNode;

    public:
        BinOp(Node *l, Node *o, Node *r);
        void addLChild(Node *l);
        void addRChild(Node *r);
};

class Number: public Node
{
    private:
        Token *number;

    public:
        Number(Token *n);
        Token *getNumber(void);
};

class Parser
{
    private:
        Lexer *lexer;
        AST *tree;
        std::string fileName;
        std::string fileText;
    public:
        Parser(Lexer *l, std::string ft, std::string fn);
        AST getTree(void);
        Error *parse(void);
};

#endif