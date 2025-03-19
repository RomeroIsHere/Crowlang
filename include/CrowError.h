#ifndef CROWERROR_H
#define CROWERROR_H
#include <stdexcept>
#include "Token.h"
class CrowError : public std::runtime_error {
    public:
        CrowError();
        CrowError(unsigned int tick);
        CrowError(unsigned int tick,int id);
        CrowError(unsigned int tick,Token t);
        unsigned int setError(unsigned char errorNumber);
        bool hasError();
        virtual const char* what();
        unsigned int code;
        int errorId;
        Token tok;
        //byte0bit(0) is Tokenizer Error(1 error)
        //byte0bit(1234567) are Syntax Errors(2^7=63 erros)
        //byte0bit(89ABCDEF) are Semantic Errors(2^8=127 errors)
        //byte1bit(12345678) are code Errors(2^8=127 errors)
        //byte1bit(9ABCDEF)-byte4 bits are Unused but reserved
    protected:

    private:
};

#endif // CROWERROR_H
