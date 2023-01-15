#ifndef PREPROCESSOR_HPP
# define PREPROCESSOR_HPP

#include <string>
#include <map>
#include <sstream>
#include "errors.hpp"

class Preprocessor
{
    public:
        Preprocessor(std::string &in);
        ~Preprocessor();
        std::map<std::string, int> process(void);

    private:
        std::string &input;
        std::string output;
        PreprocessorError *error;

        void purify_file(void);
        std::map<std::string, int> process_labels(void);
};

#endif