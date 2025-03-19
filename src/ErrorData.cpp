#include "ErrorData.h"
#include <string>
#include "CrowError.h"
#include "Token.h"

std::string parseError(CrowError err){
    std::string Error="Se esperaba Token de Tipo:";
    Error+=Token::string(static_cast<subtokentype>(err.errorId));
    Error+="\nEn vez de:"+Token::string(err.tok.subtype);
    Error+="\nEn Linea:"+std::to_string(err.tok.line);
    return Error;
}

