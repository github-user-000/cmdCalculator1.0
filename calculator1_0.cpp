// calculator program from "Programming: Principles and Practice Using C++" by "Bjarne Stroustrup"

#include <iostream>
#include <exception>
#include <string>
#include <cmath>
using namespace std;

// simple error system
void error(string str)
{
        throw runtime_error(str);
}

const char NUM = '8';
const string PROMPT = ".>>";
const char ADD = '+';
const char SUB = '-';
const char MUL = '*';
const char DIV = '/';
const char MOD = '%';
const char EQL = '=';
const char EOL = ';';
const char LPARENTHESIS = '(';
const char RPARENTHESIS = ')';

// for making all characters into certain token
class Token
{
public:
        char kind;
        double value; // if kind == number, store the value
};

// for i/o of tokens using cin
class Token_stream
{
public:
        void putback(Token); // store the token into buffer, if full == false
        Token get();         // get Token from the input stream
        void ignore(char);
private:
        bool full{false};
        Token buffer;
};

double expression();   // for addition and subtraction
double term();         // for multiplication, modulus and division
double primary();      // for floating-point values and parenthesis
void calculate();      // for printing the result

Token_stream ts;

int main()
{
        while(true)
        try{
                calculate();
        }
        catch(runtime_error& e)
        {
                cerr << e.what() << endl;
                ts.ignore(';');
        }
}

// putback the Token that is not in use
void Token_stream::putback(Token t)
{
        if(full)
                error("Buffer is full!\n");
        buffer = t;
        full = true; // now the buffer is full
}

// get the Token from the stream
Token Token_stream::get()
{
        // first check the buffer
        if(full)
        {
                full = false;
                return buffer;
        }

        // then check the input stream
        char ch;
        cin >> ch;

        switch(ch)
        {
        case EOL:
        // operators
        case ADD: case SUB:
        case MUL: case DIV:
        case MOD:
        // binders
        case LPARENTHESIS: case RPARENTHESIS:
                return Token{ch};
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case '.':
        {       // if a digit is found, the the whole number
                cin.putback(ch);
                double d;
                cin >> d;
                return Token{NUM, d}; // '8' for number kind
        }
        default:
                error("Bad Token!");
        }
}

// ignore the characters upto this certain character
void Token_stream::ignore(char c)
{
        // first empty the buffer
        if(full && c==buffer.kind)
        {
                full = false;
                return;
        }
        full = false;

        // then empty the stream
        char ch{};
        while(cin >> ch)
                if(ch==c)
                        return;

}

// for addition and subtraction
double expression()
{
        double left = term(); // get the left hand term
        Token t = ts.get();   // then get the next token
        while(true)
        {
                switch(t.kind)
                {
                case ADD:
                        left += term();
                        t = ts.get();
                        break;
                case SUB:
                        left -= term();
                        t = ts.get();
                        break;
                default:
                        ts.putback(t);
                        return left;
                }
        }
}

// similarly
double term()
{
        double left = primary();
        Token t = ts.get();
        while(true)
        {
                switch(t.kind)
                {
                case MUL:
                        left *= primary();
                        t = ts.get();
                        break;
                case DIV:
                {
                        double d = primary();
                        if(d == 0)
                                error("division by zero!");
                        left /= d;
                        t = ts.get();
                        break;
                }
                case MOD:
                {
                        double d = primary();
                        if (d == 0)
                                error("divide by zero");
                        left = fmod(left,d);
                        t = ts.get();
                        break;
                }
                default:
                        ts.putback(t);
                        return left;
                }
        }
}

double primary()
{
        Token t = ts.get();
        switch(t.kind)
        {
        case NUM: // kind for number
                return t.value;
        case SUB:
                return -primary();
        case ADD:
                return primary();
        case LPARENTHESIS:
        {
                double d = expression();
                t = ts.get();
                if(t.kind != RPARENTHESIS)
                        error("')' is expected!");
                return d;
        }
        default:
                error("bad Token!");
        }
}

void calculate()
// expression evaluation loop
{
        while (cin)
        {
                cout << PROMPT;
                Token t = ts.get();
                while (t.kind == EOL)
                        t=ts.get();
                // first discard all “prints”
                ts.putback(t);
                cout << "=" << expression() << '\n';
        }
}
