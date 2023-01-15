#include "../includes/parser.hpp"
#include <sstream>
#include <iostream>
#include <vector>

std::vector<std::string> split(std::string s, char del = ' ')
{
    std::vector<std::string> res;
    std::string tok;
    std::stringstream stream(s);

    while(std::getline(stream, tok, del))
        res.push_back(tok);
    return res;
}

std::string Parser::read_file(void)
{
    std::string line;
    std::string final_file = "";
    std::ifstream file;

    file.open(this->inFileName);
    if (!file.is_open())
        return (NULL);
    while (getline(file, line))
    {
        if (line != "")
            final_file += (line + "\n");
    }
    return (final_file);
}

bool is_number(char c)
{
    return (c >= '0' && c <= '9');
}

Parser::Parser(std::string ifn, std::string ofn)
{
    this->inFileName = ifn;
    this->outFileName = ofn;
    this->inFileContent = this->read_file();
    std::string &input = this->inFileContent;
    this->preprocessor = new Preprocessor(input);
    this->curr_line = 1;
    this->line = "";
    this->error = NULL;

    this->instructions["ADD"] = 0b01000000;
    this->instructions["SUB"] = 0b01000100;
    this->instructions["MUL"] = 0b01001000;
    this->instructions["DIV"] = 0b01001100;
    this->instructions["AND"] = 0b01010000;
    this->instructions["BOR"] = 0b01010100;
    this->instructions["XOR"] = 0b01011000;
    this->instructions["RSH"] = 0b01011100;
    this->instructions["LSH"] = 0b01100000;
    this->instructions["MOV"] = 0b01100100;
    this->instructions["CMP"] = 0b01101010;
    this->instructions["LDB"] = 0b10000000;
    this->instructions["LDH"] = 0b10000100;
    this->instructions["LDW"] = 0b10001000;
    this->instructions["STB"] = 0b10001100;
    this->instructions["STH"] = 0b10010000;
    this->instructions["STW"] = 0b10010100;
    this->instructions["PSH"] = 0b10011000;
    this->instructions["POP"] = 0b10011100;
    this->instructions["NOP"] = 0b00000000;
    this->instructions["JMP"] = 0b00000100;
    this->instructions["BRL"] = 0b00001000;
    this->instructions["BRX"] = 0b00001100;

    this->conditions["EQ"] = 0b0000;
    this->conditions["NE"] = 0b0001;
    this->conditions["CS"] = 0b0010;
    this->conditions["CC"] = 0b0011;
    this->conditions["MI"] = 0b0100;
    this->conditions["PL"] = 0b0101;
    this->conditions["VS"] = 0b0110;
    this->conditions["VC"] = 0b0111;
    this->conditions["HI"] = 0b1000;
    this->conditions["LS"] = 0b1001;
    this->conditions["GE"] = 0b1010;
    this->conditions["LT"] = 0b1011;
    this->conditions["GT"] = 0b1100;
    this->conditions["LE"] = 0b1101;
    this->conditions["AL"] = 0b1110;
}

Parser::~Parser()
{
    delete this->preprocessor;
}

void Parser::parse_failed(void)
{
    std::cerr << "Parse failed !" << std::endl;
}

void Parser::write_header(std::ofstream *output)
{
    char header[0x1000];

    // header (0x1000 first bytes) filled with 0 for now
    // except B00BBEE5.
    header[1] = 0xb0;
    header[0] = 0x0b;
    header[3] = 0xb3;
    header[2] = 0x35;
    for (int i = 4; i < 0x1000; i++)
    {
        header[i] = 0;
    }
    output->write(header, 0x1000);
}

void Parser::write_line(std::ofstream *output, uint64 line)
{
    char buff[6];

    buff[0] = (line >> 40) & 0xFF;
    buff[1] = (line >> 32) & 0xFF;
    buff[2] = (line >> 24) & 0xFF;
    buff[3] = (line >> 16) & 0xFF;
    buff[4] = (line >> 8) & 0xFF;
    buff[5] = line & 0xFF;
    output->write(buff, 6);
}

void Parser::write_last(std::ofstream *output)
{
    char buff[6];

    for (int i = 0; i < 6; i++)
        buff[i] = 0xFF;
    output->write(buff, 6);
}

void Parser::parse(void)
{
    uint64 converted_line;
    std::ofstream output;

    this->labels = this->preprocessor->process();
    std::istringstream s(this->inFileContent);
    std::cout << this->inFileContent << std::endl;
    output.open(this->outFileName);
    if (!output.is_open())
    {
        this->error = new FileError(this->curr_line, 0, this->inFileName, "Cannot open file.");
        this->error->display_error(FileError::type);
        return;
    }

    this->write_header(&output);
    // main parsing, replacing each line with it's binary equivalent
    while(!std::getline(s, this->line).eof())
    {
        converted_line = this->parse_line();
        if (this->error != NULL) // if an error is thrown
            return this->parse_failed();
        this->write_line(&output, converted_line);
        this->curr_line++;
    }
    this->write_last(&output);
    output.close();
    std::cout << "Done !" << std::endl;
}

uint64  Parser::parse_line(void)
{
    bool constant = false;
    std::vector<std::string> splitted;
    std::cout << this->line << std::endl;
    splitted = split(this->line);
    uint64 converted_line = 0;

    // parsing opcode
    if (this->instructions.find(splitted.at(0)) == this->instructions.end())
    {
        this->error = new SyntaxError(this->curr_line, 0, this->inFileName,"invalid instruction");
        this->error->display_error(SyntaxError::type);
        return (-1);
    }
    converted_line |= this->instructions[splitted.at(0)];

    // parsing S and C bits
    if (((converted_line & 0b11000000) >> 6) == 1)
    {
        if ((splitted.at(1) == "S") || splitted.at(0) == "CMP")
        {
            converted_line |= 0b00000010;
            if (splitted.at(0) != "CMP")
                splitted.erase(std::next(splitted.begin()));
        }
        if (splitted.at(1) == "C")
        {
            constant = true;
            converted_line |= 0b00000001;
            splitted.erase(std::next(splitted.begin()));
        }
    }

    // parsing DEST
    converted_line <<= 4;
    if ((converted_line & 0xC00) >> 10 == 1 || (converted_line & 0xC00) >> 10 == 2)
    {
        if (splitted.at(0) != "CMP" && splitted.at(0) != "PSH" && splitted.at(0) != "POP")
        {
            std::string reg = "";
            reg += splitted.at(1)[1];
            reg += splitted.at(1)[2];
            converted_line |= std::stoi(reg);
            splitted.erase(std::next(splitted.begin()));
        }
    }
    else if ((converted_line & 0xC00) >> 10 == 0)
    {
        if (this->conditions.find(splitted.at(1)) == this->conditions.end())
        {
            this->error = new SyntaxError(this->curr_line, 0, this->inFileName, "incorrect condition:");
            this->error->display_error(SyntaxError::type);
            return (-1);
        }
        converted_line |= this->conditions[splitted.at(1)];
        splitted.erase(std::next(splitted.begin()));
    }

    // parsing OP1
    converted_line <<= 4;
    if ((converted_line & 0xC000) >> 14 == 1)
    {
        if (splitted.at(0) != "MOV")
        {
            std::string reg = "";
            reg += splitted.at(1)[1];
            reg += splitted.at(1)[2];
            converted_line |= std::stoi(reg);
            splitted.erase(std::next(splitted.begin()));
        }
    }

    // parsing OP2
    converted_line <<= 4;
    if ((converted_line & 0xC0000) >> 18 == 1 && !constant)
    {
        std::string reg = "";
        reg += splitted.at(1)[1];
        reg += splitted.at(1)[2];
        converted_line |= std::stoi(reg);
        splitted.erase(std::next(splitted.begin()));
    }

    converted_line <<= 28;
    if ((converted_line & 0xC00000000000) >> 46 == 1 && constant)
    {
        converted_line |= std::stoi((std::string) splitted.at(1));
        splitted.erase(std::next(splitted.begin()));
    }
    else if (splitted.at(0) == "JMP" || splitted.at(0) == "BRX")
    {
        if (is_number(splitted.at(1)[0]))
            converted_line |= std::stoi((std::string) splitted.at(1));
        else if (this->labels.find(splitted.at(1)) != this->labels.end())
            converted_line |= this->labels[splitted.at(1)];
        else 
        {
            this->error = new SyntaxError(this->curr_line, 0, this->inFileName, "label doesn't exists.");
            this->error->display_error(SyntaxError::type);
            return (-1);
        }
        splitted.erase(std::next(splitted.begin()));
    }
    else if ((converted_line & 0xC00000000000) >> 46 == 2)
    {
        if (splitted.at(0) == "PSH" || splitted.at(0) == "POP")
        {
            int mask = 0;
            while (splitted.at(1) != splitted.back())
            {
                mask |= 1 << (15 - std::stoi(splitted.at(1)));
                splitted.erase(std::next(splitted.begin()));
            }
            mask |= 1 << (15 - std::stoi(splitted.at(1)));
            splitted.erase(std::next(splitted.begin()));
            converted_line |= (mask << 12);
        }
        else
        {
            converted_line |= std::stoi((std::string) splitted.at(1));
            splitted.erase(std::next(splitted.begin()));
        }
    }
    return converted_line;
}