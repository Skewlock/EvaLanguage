#include "includes/main.hpp"
#include "includes/parser.hpp"
#include "includes/errors.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    std::string inFileName;
    std::string outFileName;
    std::string fileContent;
    Error *error;
    Parser *parser;
    switch (argc)
    {
        case 1:
            error = new FileError(0, 0, "File", "No input file.");
            error->display_error(FileError::type);
            return (1);
            break;
        
        case 2:
            inFileName = std::string(argv[1]);
            outFileName = "a.out";
            break;
        
        case 3:
            inFileName = std::string(argv[1]);
            outFileName = std::string(argv[2]);
            break;
    }
    parser = new Parser(inFileName, outFileName);
    parser->parse();
    delete parser;
    return (0);
}