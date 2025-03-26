#include <iostream>
#include <algorithm>
#include <string>
#include <stdint.h>
#include <limits>
#include "TypeChecker.h"
#include "Token.h"
#include "CrowError.h"
#include "HelperFunctions.h"
#include "SymbolTable.h"
int64_t evaluate(Token t){
    int64_t returnal= -8589934592;
    std::string representation=t.tokenString;
    if(!(t.subtype==CHAIN||t.subtype==CHARACTER))
        representation=replaceAll(t.tokenString,"_","");

    if(t.subtype==DECIMALVALUE)
        returnal=std::stoi(representation);
    else if(t.subtype==HEXADECIMAL){
        representation=replaceAll(t.tokenString,"h","");
        returnal=std::stoi(representation,nullptr,16);
    }else if(t.subtype==BINARY){
        representation=replaceAll(t.tokenString,"b","");
        returnal=std::stoi(representation,nullptr,2);
    }else if(t.subtype==FIXEDPOINT){
        representation=replaceAll(t.tokenString,"f","");
        representation=replaceAll(t.tokenString,".","");
        returnal=std::stoi(representation,nullptr,16);
        }
    return returnal;
}

TypeChecker::TypeChecker()
{
    //ctor
}

TypeChecker::~TypeChecker()
{
    //dtor
}

void TypeChecker::expressionRules(std::vector<Token> tokenList){
    literalValueRanges(&tokenList);
    findAllIdentifiers(tokenList);
}



CrowError TypeChecker::literalValueRanges(std::vector<Token> *allToken){
    std::vector<int> valueList;//index
    std::vector<Token>& tokenList=*allToken;
    int size=tokenList.size();
    for(int ii=0;ii<size;ii++){
        if(tokenList[ii].type==VALUE){
         valueList.push_back(ii);
        }
    }
    int64_t temp;
    for(int index:valueList){
        Token t=tokenList[index];
            try{
                temp=evaluate(t);
            }catch(std::out_of_range const&){
                return CrowError(3,t);
            }catch(std::invalid_argument const&){
                return CrowError(3,t);
            }

        switch(t.subtype){
            case DECIMALVALUE:
            case BINARY:
            case FIXEDPOINT:
            case HEXADECIMAL:
            default:
                //std::cout<<Token::string(t.subtype)<<":"<<t.tokenString<<" Value "<<temp<<'\n';
                if (temp>INT_MAX||temp<INT_MIN)
                    return CrowError(3,t);
                else
                    tokenList[index].tokenString=std::to_string(temp);
                break;
            case CHAIN:
            case CHARACTER:
                //std::cout<<Token::string(t.subtype)<<":"<<t.tokenString<<'\n';
            break;
        }
    }
    return CrowError();
}

CrowError TypeChecker::findAllIdentifiers(std::vector<Token> allToken){
    std::vector<Token> definedList;
    scope scp=GLOBAL;
    std::string upper="";
    for(int ii=0;ii<(int)allToken.size();ii++){
        if(allToken[ii].type==TYPE&&allToken[ii+1].subtype==IDENT){
                definedList.push_back(allToken[ii+1]);
                allIdentifiers.add((Symbol){&allToken[ii+1], allToken[ii].subtype,scp,upper});
        }
        switch(allToken[ii].subtype){
            case ARR:
            case POINTER:
                allIdentifiers.add((Symbol){&allToken[ii+2], allToken[ii].subtype,scp,upper});
                ii++;
            break;
            case FUNCTION:
                allIdentifiers.clearLocal();
                allIdentifiers.add((Symbol){&allToken[ii+1], allToken[ii].subtype,GLOBAL,""});
                scp=LOCAL;
                break;
            case MACRO:
                allIdentifiers.add((Symbol){&allToken[ii+1], allToken[ii].subtype,GLOBAL,""});
                break;
            case STRUCTURE:
                scp=STRUCTSCOPE;
                upper=allToken[++ii].tokenString;
                allIdentifiers.add((Symbol){&allToken[ii], allToken[ii-1].subtype,GLOBAL,""});
                    do{
                        if(allToken[ii].subtype==POINTER){
                            allIdentifiers.add((Symbol){&allToken[ii+2], allToken[ii].subtype,scp,upper});
                            ii++;
                        }else if(allToken[ii].type==TYPE){
                            allIdentifiers.add((Symbol){&allToken[ii+1], allToken[ii].subtype,STRUCTSCOPE,upper});
                        }else if(allToken[ii].subtype==STRUCTDECLARE){
                            allIdentifiers.add((Symbol){&allToken[ii+2], allToken[ii].subtype,STRUCTSCOPE,allToken[ii+1].tokenString},upper);
                        }
                        ii++;
                    }while(allToken[ii+1].subtype!=CLOSECURL);
                    upper.clear();
                break;
            case STRUCTDECLARE:
                allIdentifiers.add((Symbol){&allToken[ii+2], allToken[ii+1].subtype,scp,allToken[ii+1].tokenString});
                break;
            case OPENCURL:
                    allIdentifiers.add((Symbol){&allToken[ii], allToken[ii].subtype,LOCAL,""});
                break;
            case CLOSECURL:
                allIdentifiers.clearLocal();
                if (!allIdentifiers.find(Token("{")))
                    scp=GLOBAL;
                break;
            case IDENT:
                if (!(allIdentifiers.find(allToken[ii]))){

                        //std::find_if(definedList.begin(), definedList.end(),[allToken,ii](Token in){ return in.tokenString==allToken[ii].tokenString;}) == definedList.end()
                        //std::cout<<"Was Not found in global or Local";
                        if(!allIdentifiers.find(allToken,ii)){
                            //allIdentifiers.print();
                            return CrowError(4,allToken[ii]);
                        }
                    }
            break;
            case OPENBRACKET:
                if(!allIdentifiers.isArrayAccesible(allToken, ii-1)){
                    return CrowError(5,allToken[ii-1]);
                }
            default:
            break;
        }
    }

    return CrowError();
}
