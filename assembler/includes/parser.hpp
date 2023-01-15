#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include "errors.hpp"
#include "preprocessor.hpp"

typedef unsigned long long uint64;
typedef unsigned long uint32;

class Parser
{
    public:
        Parser(std::string ifn, std::string ofn);
        ~Parser();
        void parse(void);
        uint64 parse_line(void);
        std::string read_file(void);
        void parse_failed(void);
        

    private:
        int curr_line;
        std::string line;
        std::string inFileContent;
        std::string outFileName;
        std::string inFileName;
        std::map<std::string, int> instructions;
        std::map<std::string, int> conditions;
        std::map<std::string, int> labels;
        Preprocessor *preprocessor;
        Error *error;
        void write_header(std::ofstream *output);
        void write_line(std::ofstream *output, uint64 line);
        void write_last(std::ofstream *output);
};

#endif