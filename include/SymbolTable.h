#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "Token.h"
#include <string>
#include <vector>
#include <map>
enum scope{
    GLOBAL=0,
    LOCAL,
    STRUCTSCOPE
};

struct Symbol{
    Token *tok;
    subtokentype representation;
    scope scp;
    std::string uppername;
    /*
        Function call only
    VOID,
        Valid Types
    ARR,
    STRUCTDECLARE,
    VOL,
    POINTER,
    U8,
    U16,
    U32,
    S8,
    S16,
    S32
    //*/
};
//*/
class SymbolTable
{
    public:
        SymbolTable();
        virtual ~SymbolTable();
    void add(Symbol s);
    void add(Symbol s,std::string addToStruct);
    void print();
    bool find(Token t);
    bool find(std::vector<Token> everyOtherToken,int ii);
    bool isArrayAccesible(Token t);
    bool isArrayAccesible(std::vector<Token> everyOtherToken, int ii);
    void clearLocal();
    Symbol* findtype(std::vector<Token> everyOtherToken, int ii);
    protected:
    std::vector<Symbol> GlobalIdentifiers;
    std::vector<Symbol> LocalIdentifiers;
    std::map<std::string/*Expanded Struct Name*/,std::vector<Symbol>> StructMap;
    private:
};

#endif // SYMBOLTABLE_H
