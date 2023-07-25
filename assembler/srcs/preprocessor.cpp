#include "../includes/preprocessor.hpp"
#include <iostream>


Preprocessor::Preprocessor(std::string &in):
input(in)
{
    this->error = NULL;
    this->output = "";
}

Preprocessor::~Preprocessor()
{
    if (this->error != NULL)
        delete this->error;
}

std::vector<std::string> split(std::string s, char del = ' ')
{
    std::vector<std::string> res;
    std::string tok;
    std::stringstream stream(s);

    while(std::getline(stream, tok, del))
        res.push_back(tok);
    return res;
}

bool is_whitespace(char c)
{
    return ((c > 7 && c < 14) || c == 32);
}

std::string to_upper(std::string str)
{
    for (unsigned long int i = 0; i < str.length(); i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] = str[i] - 32;
    }
    return (str);
}

void Preprocessor::purify_file(void)
{
    std::stringstream stream(this->input);
    std::string line;
    std::string final_text = "";
    int j = 0;
    while (getline(stream, line))
    {
        while (is_whitespace(line[j]))
            j++;
        line.erase(0, j);
        final_text += line;
        final_text += "\n";
    }
    this->input = to_upper(final_text);
}

std::map<std::string, uint32> Preprocessor::process_labels(void)
{
    int count = 0;
    uint64 i = 0;
    std::map<std::string, uint32> labels;
    std::string label;
    while (this->input[i])
    {
        if (is_whitespace(this->input[i]))
        {
            if (this->input[i] == '\n')
                count++;
            label = "";
        }
        else if (this->input[i] == ':')
        {
            label = to_upper(label);
            labels[label] = (count * 6) + DATA_START + this->data_size + this->bss_size;
            this->input.erase(i - label.size(), label.size() + 2);
            count--;
        }
        else
            label += this->input[i];
        i++;
    }
    return labels;
}

std::map<std::string, uint64> Preprocessor::process_data(void)
{
    uint64 i = 0;
    std::map<std::string, uint64> data;
    std::stringstream stream(this->input);
    std::string strline;
    std::vector<std::string> line;
    while (getline(stream, strline))
    {
        line = split(strline);
        if (line.at(0) == ".equ")
        {
            if (std::size(line) < 3)
            {
                this->error = new PreprocessorError(i, 0, "<stdin>", "invalid pseudo-instruction");
                return;
            }
            data[line.at(1)] = stoi(line.at(2));
        }
        i++;
    }
    return data;
}
/*
std::map<std::string, uint32> Preprocessor::process_bss(void)
{
    uint64 i = 0;
    std::map<std::string, uint32> bss;
    std::stringstream stream(this->input);
    std::string strline;
    std::vector<std::string> line;
    while (getline(stream, strline))
    {
        line = split(strline);
        if (line.at(0) == ".short")
        {
            if (std::size(line) < 2)
            {
                this->error = new PreprocessorError(i, 0, "<stdin>", "invalid pseudo-instruction");
                return;
            }
            bss[line.at(1)] = stoi(line.at(2));
        }
        i++;
    }
    return bss;
}*/

void Preprocessor::process(void)
{
    std::map<std::string, uint32> labels;
    std::map<std::string, uint64> data;
    std::map<std::string, uint32> bss;
    labels = this->process_labels();
    data = this->process_data();
    //bss = this->process_bss();
    this->purify_file();
}