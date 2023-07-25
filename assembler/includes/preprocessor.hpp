#ifndef PREPROCESSOR_HPP
# define PREPROCESSOR_HPP

#include <string>
#include <map>
#include <sstream>
#include <vector>
#include "errors.hpp"
#include "general.hpp"

class Preprocessor
{
    public:
        Preprocessor(std::string &in);
        ~Preprocessor();
        void process(void);
        std::map<std::string, uint32> process_labels(void);
        std::map<std::string, uint64> process_data(void);
        //std::map<std::string, uint32> process_bss(void);

    private:
        std::string &input;
        std::string output;
        PreprocessorError *error;
        uint64 data_size;
        uint64 bss_size;

        void purify_file(void);
};

#endif