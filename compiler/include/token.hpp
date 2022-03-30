#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <string>

enum token_types
{
    TT_INT,
    TT_FLOAT,
    TT_OPERATION,
    TT_LPAREN,
    TT_RPAREN,
    TT_EOF
};

enum operation_types
{
    TT_PLUS,
    TT_MINUS,
    TT_DIV,
    TT_MUL,
    TT_MOD
};

class Token
{
    protected:
        int type;

    public:
        Token(token_types type);
        int getType(void);
        virtual std::string toString(void);
};

class FloatToken: public Token
{
    private:
        float value;
    
    public:
        FloatToken(token_types t, float v);
        float getValue(void);
        void setValue(float v);
        std::string toString(void) override;
};

class IntToken: public Token
{
    private:
        int value;
    
    public:
        IntToken(token_types t, int v);
        int getValue(void);
        void setValue(int v);
        std::string toString(void) override;
};

class OpToken: public Token
{
    private:
        operation_types value;
    
    public:
        OpToken(token_types t, operation_types v);
        operation_types getValue(void);
        void setValue(operation_types v);
        std::string toString(void) override;
};

#endif