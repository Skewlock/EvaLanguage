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
    unsigned long long i = 0;
    int j = 0;
    while (getline(stream, line))
    {
        while (is_whitespace(line[j]))
            j++;
        line.erase(0, j);
        final_text += line;
        final_text += "\n";
        i++;
    }
    this->input = to_upper(final_text);
}

std::map<std::string, int> Preprocessor::process_labels(void)
{
    int count = 0;
    long int i = 0;
    std::map<std::string, int> labels;
    std::string label;
    std::istringstream s(this->input);
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
            labels[label] = (count * 6) + 0x1000;
            this->input.erase(i - label.size(), label.size() + 2);
            count--;
        }
        else
            label += this->input[i];
        i++;
    }
    return labels;
}

std::map<std::string, int> Preprocessor::process(void)
{
    std::map<std::string, int> labels;
    labels = this->process_labels();
    this->purify_file();
    return labels;
}