#include "SymbolTable.h"
#include <iostream>
#include <algorithm>
SymbolTable::SymbolTable()
{
    //ctor
}

SymbolTable::~SymbolTable()
{
    //dtor
}
void SymbolTable::add(Symbol s,std::string addToStruct){
    std::map<std::string, std::vector<Symbol>>::iterator index = StructMap.find(addToStruct);
    if (index== StructMap.end())
        StructMap[addToStruct];
    StructMap[addToStruct].push_back(s);
}
void SymbolTable::add(Symbol s){
    switch(s.scp){
    case GLOBAL:
        GlobalIdentifiers.push_back(s);
    break;
    case LOCAL:
        LocalIdentifiers.push_back(s);
        break;
    case STRUCTSCOPE:
        std::map<std::string, std::vector<Symbol>>::iterator index = StructMap.find(s.uppername);
        if (index== StructMap.end())
            StructMap[s.uppername];
        StructMap[s.uppername].push_back(s);
        break;
    }
}

void SymbolTable::clearLocal(){
    while(!LocalIdentifiers.empty()&&LocalIdentifiers.back().tok->subtype!=OPENCURL)
    {
        LocalIdentifiers.pop_back();
    }
    if(!LocalIdentifiers.empty())
        LocalIdentifiers.pop_back();
}
void SymbolTable::print(){
    std::cout<<"Global"<<"\n";
    for(Symbol s:GlobalIdentifiers)
        s.tok->print();
    std::cout<<"Local"<<"\n";
    for(Symbol s:LocalIdentifiers)
        s.tok->print();
    std::cout<<"Structura"<<"\n";
    for(const auto& [name, vec]:StructMap){
        std::cout<<name<<"\n";
        for(Symbol s:vec)
            s.tok->print();
        }
}


bool SymbolTable::find(Token t){
    //std::cout<<"\n\n    FINDING:"<<t.tokenString;
    auto localic=std::find_if(LocalIdentifiers.begin(), LocalIdentifiers.end(),[t](Symbol in){ return in.tok->tokenString==t.tokenString;});
    auto globalic= std::find_if(GlobalIdentifiers.begin(), GlobalIdentifiers.end(),[t](Symbol in){ return in.tok->tokenString==t.tokenString;});
    return localic != LocalIdentifiers.end()
    || globalic != GlobalIdentifiers.end();

}

Symbol* SymbolTable::findtype(std::vector<Token> everyOtherToken, int ii){//returns the String name of the struct
    Token t=everyOtherToken[ii];
    Symbol *structSymbol;
    std::string structName;

    if(everyOtherToken[ii-1].subtype==DOT){
            structSymbol=findtype(everyOtherToken,ii-2);

        if(structSymbol!=nullptr){
                structName=structSymbol->uppername;
            std::vector<Symbol>::iterator found=std::find_if(StructMap[structName].begin(), StructMap[structName].end(),[t](Symbol in){ return in.tok->tokenString==t.tokenString;});
            if( found==StructMap[structName].end())
                return nullptr;
            else{
                    /*
                    std::cout<<"\n\nFound Branch:"<<found->tok->tokenString;
                    std::cout<<"\nFound in Table:"<<structName;
                    std::cout<<"\nType in:"<<found->uppername;
                   found->tok->print();*/
                return &(*found);
                }
        }
    }else{

        std::vector<Symbol>::iterator elementFound=std::find_if(LocalIdentifiers.begin(), LocalIdentifiers.end(),[t](Symbol in){ return in.tok->tokenString==t.tokenString;});
        if(elementFound != LocalIdentifiers.end()){
                /*std::cout<<"\n\nFound Root:"<<elementFound->tok->tokenString;
                std::cout<<"\nFound Table:"<<elementFound->uppername;
                elementFound->tok->print();*/

            return &(*elementFound);
        }
    }
    return nullptr;
}
bool SymbolTable::find(std::vector<Token> everyOtherToken, int ii){
    //std::cout<<"Searching for:"<<everyOtherToken[ii].tokenString<<"\n";
    std::string type;
    Symbol *point=findtype(everyOtherToken,ii);
    if(point!=nullptr)
        type=point->uppername;
    return type!="";
}
bool SymbolTable::isArrayAccesible(Token t){
    auto localic=std::find_if(LocalIdentifiers.begin(), LocalIdentifiers.end(),[t](Symbol in){ return (in.representation==ARR||in.representation==POINTER)&& in.tok->tokenString==t.tokenString;});
    auto globalic= std::find_if(GlobalIdentifiers.begin(), GlobalIdentifiers.end(),[t](Symbol in){ return (in.representation==ARR||in.representation==POINTER)&&in.tok->tokenString==t.tokenString;});
    //std::cout<<"\nRan Array Access on:"<<t.tokenString<<"\n";
    //t.print();
    return (localic != LocalIdentifiers.end()
    || globalic != GlobalIdentifiers.end());
}

bool SymbolTable::isArrayAccesible(std::vector<Token> everyOtherToken, int ii){
    bool simple=isArrayAccesible(everyOtherToken[ii]);
    if(!simple){
    Symbol *Data=findtype(everyOtherToken,ii);
    simple=Data->representation==POINTER||Data->representation==ARR;
    }
    return simple;
}
