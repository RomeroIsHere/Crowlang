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

    private:
};

#endif // TRANSLATOR_H
