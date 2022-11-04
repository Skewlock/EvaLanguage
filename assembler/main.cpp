#include "includes/main.hpp"
#include "includes/parser.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    std::string inFileName;
    std::string outFileName;
    std::string fileContent;
    Parser *parser;
    if (argc != 3)
        return (1);
    inFileName = std::string(argv[1]);
    outFileName = std::string(argv[2]);
    parser = new Parser(inFileName, outFileName);
    parser->parse();
    return 0;
}