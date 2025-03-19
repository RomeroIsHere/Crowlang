#ifndef TYPECHECKER_H
#define TYPECHECKER_H
#include <vector>
#include <stdint.h>
#include "Token.h"
#include "CrowError.h"
#include "SymbolTable.h"

int64_t evaluate(Token t);
class TypeChecker
{
    public:
        SymbolTable allIdentifiers;
        TypeChecker();
        virtual ~TypeChecker();
        void expressionRules(std::vector<Token> tokenList);
        CrowError findAllIdentifiers(std::vector<Token> allToken);
        CrowError literalValueRanges(std::vector<Token> LiteralValues);

        CrowError structConstruct();
        CrowError accesArrayOrPointer();
        CrowError functionCall();

};

#endif // TYPECHECKER_H
