/**
 * @file main.cpp
 * @author Mathias Couriol (mathias.couriol@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "include/compiler.hpp"

using namespace std;

/**
 * @brief run the compiler
 * 
 * @param input 
 * @param fileName 
 */
void run(std::string input, std::string fileName)
{
    Lexer *lexer = new Lexer(input, fileName);
    Parser *parser = new Parser(lexer, input, fileName);
    Error *error = NULL;
    error = parser->parse();
    if (error != NULL)
    {
        std::cout << error->toString() << std::endl;
        return;
    }
}

/**
 * @brief read file or input and run the compiler
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{
    string input;
    string fileName;
    if (argc < 2)
    {
        fileName = "<stdin>";
        while (1)
        {
            cout << "boop> ";
            getline(cin, input);
            run(input, fileName);
        }
    }
    else
    {
        fileName = argv[1];
        std::ifstream ifs(fileName);
        input.assign((std::istreambuf_iterator<char>(ifs)),
                    (std::istreambuf_iterator<char>()));
        run(input, fileName);
    }
    return (0);
}