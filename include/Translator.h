#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include "Token.h"
#include <fstream>
#include <string>
#include <vector>

class Translator
{
    public:
    std::ofstream Output;
        Translator(std::string OutputName);
        virtual ~Translator();
    void translate(std::vector<Token> tokenList);

    protected:
        std::string tillLine(std::vector<Token>* tokenIn, int* in);
        std::string ifcode(std::vector<Token>* tokenIn, int* in);
        std::string loopcode(std::vector<Token>* tokenIn, int* in);
        std::string typedVariable(std::vector<Token>* tokenIn, int* in);
        std::string codeblock(std::vector<Token>* tokenIn, int* in);
        std::string function(std::vector<Token> *tokenIn, int *in);
        std::string structConstruct(std::vector<Token> *tokenIn, int *in);
    private:
        std::vector<std::string> MainFunctions;

};

#endif // TRANSLATOR_H
