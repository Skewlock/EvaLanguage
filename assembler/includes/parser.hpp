#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <fstream>
#include <map>

typedef unsigned long long uint64;
typedef unsigned long uint32;

class Parser
{
    public:
        Parser(std::string ifn, std::string ofn);
        void parse(void);
        uint64 parse_line(void);
        std::string purify_file(void);
        void parse_failed(void);
        

    private:
        int curr_line;
        std::string line;
        std::string inFileContent;
        std::string outFileName;
        std::string inFileName;
        std::map<std::string, int> instructions;
        std::map<std::string, int> conditions;
};

#endif