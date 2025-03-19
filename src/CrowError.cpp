#include "CrowError.h"
#define BIT(x) 1<<(x)
#include "Token.h"

int mask(int e, int m) {return (e)&(m);}

unsigned int bitmask(int low,int high){
    //makes a mask from a to b where a is the low bits and b is high, where bitmask(0,0) makes an empty mask and bitmask(0,1) returns 1, and bitmask(1,1) returns 0
    unsigned int mask=0;
    for(int i=low;i<high;i++){
        mask|=1<<i;
    }
    return mask;
}

CrowError::CrowError(unsigned int tick):runtime_error("No Specified Error"){
    code=tick;
    errorId=-1;
    tok=Token();
}
CrowError::CrowError(unsigned int tick,int id):runtime_error("No Specified Error"){
    code=tick;
    errorId=id;
    tok=Token();
}
CrowError::CrowError(unsigned int tick, Token t):runtime_error("No Specified Error"){
    code=tick;
    errorId=tick;
    tok=t;
}
CrowError::CrowError():runtime_error("No Specified Error"){
    code=0;
    errorId=0;
    tok=Token();
}

unsigned int CrowError::setError(unsigned char errorNumber){
    code=errorNumber;
    return code;
}
bool CrowError::hasError(){
    return (bool)code;
}

const char* CrowError::what(){
    std::string explain="The Unsettled Mind is at times an ally";
    switch(code){
        default:
        case 0: explain="No Error Found";
        break;
        case 1: explain="Error de tokenizacion";
        break;
        case 2: explain="Error Sintactico";
        break;
        case 4:
        case 3: explain="Error Semantico";
        break;
    }

    return explain.c_str();
}
