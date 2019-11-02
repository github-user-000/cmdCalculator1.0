#include <iostream>
#include <exception>
#include <string>
using namespace std;

// for handling simple error
inline void error(const string& s)
{
	throw runtime_error(s);
}

const char NUMBER = '8';
const string PROMPT = ".>>";
const char EOL = ';';
const char ADD = '+';
const char SUB = '-';
const char MUL = '*';
const char DIV = '/';
const char MOD = '%';
const char EQL = '=';
const char LPARENTHESIS = '(';
const char RPARENTHESIS = ')';

// for changing characters in Token class
class Token
{
public:
        Token(char k)
                : kind{ k }
        {}
        Token(char k, double v)
                : kind{ k }, value{v}
        {}

        char kind;
        double value; // if a NUMBER then includes a value
};

// input stream for Token(s) using cin
class TokenStream
{
public:
        void putback(Token);
        Token get();
        void ignore(Token);

private:
        bool full{ false };   // at start buffer is not full
        Token buffer;         // store the required Token through putback()
};

double expression();  // for addition and subtraction
double term();        // for division, modulus and multiplication
double primary();     // for floating point numbers and parenthesis
void calculate();     // a different fuction to call expression

TokenStream ts;

int main()
{}

// to store a Token if not in use
void TokenStream::putback(Token t)
{
        if(full)
                error("buffer is full!");
        buffer = t;
        full = true; // now the buffer is full
}

// to get a Token from input stream
void TokenStream::get()
{
        if(full)
        {
                full = false;
                return buffer;
        }

        char ch;
        cin >> ch;

        switch(ch)
        {
        case EOL: case ADD:
        case SUB: case MUL:
        case DIV: case MOD:
        case LPARENTHESIS: case RPARENTHESIS:
                return Token{ ch };
        case '.': case '0':
        case '1': case '2':
        case '3': case '4':
        case '5': case '6':
        case '7': case '8':
        case '9':
        {
                cin.putback(ch);
                double d{};
                cin >> d;
                return Token{ ch, d };
        }
        default:
                error("Bad Token!");
        }
}

// ignore all Tokens upto Token 't'
void TokenStream::ignore(Token t)
{
        // first remove from buffer
        if(full && buffer.kind == t.kind)
        {
                full = false;
                return;
        }
        full = false;

        // remove from the input-stream(cin);
        char ch{};
        while(cin >> ch)
                if(ch == t.kind)
                        return;
}
