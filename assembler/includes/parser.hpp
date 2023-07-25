#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include "errors.hpp"
#include "preprocessor.hpp"
#include "general.hpp"

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
        std::map<std::string, uint8> instructions;
        std::map<std::string, uint8> conditions;
        std::map<std::string, uint32> labels;
        std::map<std::string, uint32> bss;
        std::map<std::string, uint64> data;
        Preprocessor *preprocessor;
        Error *error;
        void write_header(std::ofstream *output);
        void write_line(std::ofstream *output, uint64 line);
        void write_last(std::ofstream *output);
};

#endif