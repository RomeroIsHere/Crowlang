#ifndef SYNTAXER_H
#define SYNTAXER_H
#include <vector>
#include <functional>
#include <fstream>
#include "Token.h"
#include "CrowError.h"

struct Syntaxer{
    public:
        Syntaxer();
        Syntaxer(std::vector<Token> inList);
        Syntaxer(std::vector<Token> inList, std::string file);
        CrowError File();
        CrowError IDENTIFIER();
        CrowError reserveddeclarations();
        CrowError inclusion();
        CrowError macro();
        CrowError structDeclaration();
        CrowError funcDeclaration();
        CrowError parameterdeclaration();
        CrowError SCHEDULE();
        CrowError block();
        CrowError instruction();
        CrowError statement();
        CrowError returnal();
        CrowError assigner();
        CrowError control();
        CrowError innerdeclarations();
        CrowError variabledeclaration();
        CrowError initialization();
        CrowError calculated();
        CrowError term();
        CrowError magnitude();
        CrowError amount();
        CrowError factor();
        CrowError bits();
        CrowError unarysigned();
        CrowError OPERAND();
        CrowError comparative();
        CrowError bitshift();
        CrowError additive();
        CrowError multiplicative();
        CrowError logical();

        CrowError IFCODE();
        CrowError LOOPCODE();
        CrowError UNARYOPERATOR();
        CrowError VALUE();
        CrowError arrayc();
        CrowError TYPE();
        CrowError TYPEMOD();
        CrowError COMMENT();

        unsigned int workingindex;
        unsigned int max;
        std::vector<Token> tokenlist;
    private:
        CrowError NoneOrMore(std::function<CrowError()> functions[],int steps);
        CrowError NoneOrOnce(std::function<CrowError()> functions[],int steps);
        CrowError Once(std::function<CrowError()> functions[],int steps);
        CrowError OneOf(std::function<CrowError()> functions[],int steps);
        CrowError matchSubtype(Token t, subtokentype comp);
    int stacker;
    std::ofstream printerout;
    unsigned int maxDepth;
    CrowError maxError;
};

#endif // SYNTAXER_H
