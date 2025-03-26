#include "Token.h"
#include <string>
#include <iostream>
#include <regex>
std::map<tokentype,std::regex> regexSuperType;
std::map<subtokentype,std::regex> regexSubType;

void Token::initialize(){
    regexSuperType[COMMENT]=std::regex("(//.*)$|(/\\*.*\\*/)");
    regexSuperType[RESERVED]=std::regex("function|structure|macro|add|subroutine|frame|main|loop|if|else|return");
    regexSuperType[TYPEMOD]=std::regex("volatile|array|pointer|dstruct");
    regexSuperType[VALUE]=std::regex("0(x|h)[a-fA-F0-9_]+|0b[10_]+|0f[a-fA-F0-9]{2}\\.[a-fA-F0-9]{2}|\".*\"|'.'|[0-9]+");
    regexSuperType[TYPE]=std::regex("(u|s|bit)(8|16|32)|void|bool");
    regexSuperType[IDENTIFIER]=std::regex("[a-zA-Z][a-zA-Z0-9_]*");
    regexSuperType[PUNCTUATION]=std::regex("<<?|>>?|\\+\\+?|\\{|\\}|\\[|\\]|\\(|\\)|=|\\-\\-?|\\*|/|%|&|\\||@|\\^|_|\\.|,|;|:|~");

    regexSubType[MULTILINE]=std::regex("/\\*(\n|\r|\n\r|.)*?\\*/");
    regexSubType[MONOLINE]=std::regex("//.*");
    regexSubType[FUNCTION]=std::regex("function");
    regexSubType[RETURNAL]=std::regex("return");
    regexSubType[STRUCTURE]=std::regex("structure");
    regexSubType[MACRO]=std::regex("macro");
    regexSubType[ADD]=std::regex("add");
    regexSubType[SUBROUTINE]=std::regex("subroutine");
    regexSubType[FRAME]=std::regex("frame");
    regexSubType[MAIN]=std::regex("main");
    regexSubType[LOOP]=std::regex("loop");
    regexSubType[IFC]=std::regex("if");
    regexSubType[ELSEC]=std::regex("else");
    regexSubType[DOT]=std::regex("\\.");
    regexSubType[COMMA]=std::regex(",");
    regexSubType[OPENBRACKET]=std::regex("\\[");
    regexSubType[CLOSEBRACKET]=std::regex("\\]");
    regexSubType[OPENCURL]=std::regex("\\{");
    regexSubType[CLOSECURL]=std::regex("\\}");
    regexSubType[OPENPARENTHESIS]=std::regex("\\(");
    regexSubType[CLOSEPARENTHESIS]=std::regex("\\)");
    regexSubType[COLON]=std::regex(":");
    regexSubType[LINEEND]=std::regex(";");
    regexSubType[EQUAL]=std::regex("=");
    regexSubType[INCREASE]=std::regex("\\+\\+");
    regexSubType[DECREASE]=std::regex("\\-\\-");
    regexSubType[SUM]=std::regex("\\+");
    regexSubType[SUBTRACT]=std::regex("\\-");
    regexSubType[MULTIPLY]=std::regex("\\*");
    regexSubType[DIVIDE]=std::regex("/");
    regexSubType[MODULO]=std::regex("%");
    regexSubType[AND]=std::regex("&");
    regexSubType[OR]=std::regex("\\|");
    regexSubType[XOR]=std::regex("\\^");
    regexSubType[LESS]=std::regex("<");
    regexSubType[MORE]=std::regex(">");
    regexSubType[SHIFTRIGHT]=std::regex(">>");
    regexSubType[SHIFTLEFT]=std::regex("<<");
    regexSubType[CHAIN]=std::regex("\".*\"");
    regexSubType[CHARACTER]=std::regex("'.'");
    regexSubType[NOT]=std::regex("~");
    regexSubType[DECIMALVALUE]=std::regex("[0-9]+");
    regexSubType[BINARY]=std::regex("0b[01_]+");
    regexSubType[HEXADECIMAL]=std::regex("(0x|0h)[a-fA-F0-9_]+");
    regexSubType[FIXEDPOINT]=std::regex("0f[a-fA-F0-9]{2}\\.[a-fA-F0-9]{2}");
    regexSubType[VOID]=std::regex("void");
    regexSubType[VOL]=std::regex("volatile");
    regexSubType[ARR]=std::regex("array");
    regexSubType[POINTER]=std::regex("pointer");
    regexSubType[STRUCTDECLARE]=std::regex("dstruct");
    regexSubType[U8]=std::regex("u8|bit8");
    regexSubType[U16]=std::regex("u16|bit16");
    regexSubType[U32]=std::regex("u32|bit32");
    regexSubType[S8]=std::regex("s8");
    regexSubType[S16]=std::regex("s16");
    regexSubType[S32]=std::regex("s32");
    regexSubType[IDENT]=std::regex("[a-zA-Z][a-zA-Z0-9_]*");
}

Token::Token()
{
    id=0;
    line=0;
    tokenString="";
    type=NONE;
    subtype=Nix;
}
Token::Token(std::string stringToken)
{
    id=0;
    line=0;
    tokenString=stringToken;
    type=NONE;
    subtype=Nix;
}
Token::Token(int id, int line, std::string stringToken)
{
    this->id=id;
    this->line=line;
    this->tokenString=stringToken;
    type=NONE;
    subtype=Nix;
}
std::string stringer(tokentype t){
    std::string ret="";
    switch(t){
        case COMMENT:
            ret="COMMENT";
            break;
        case RESERVED:
            ret="RESERVED";
            break;
        case PUNCTUATION:
            ret="PUNCTUATION";
            break;
        case VALUE:
            ret="VALUE";
            break;
        case TYPE:
            ret="TYPE";
            break;
        case IDENTIFIER:
            ret="IDENTIFIER";
            break;
        case TYPEMOD:
            ret="Modificación de Tipo";
            break;
        case NONE:
            ret="TOKEN DESCONOCIDO";
        break;
    }
    return ret;
}

std::string Token::string(subtokentype t){
    std::string ret="";
    switch(t){
        case MULTILINE:
            ret="MULTILINE";
            break;
        case MONOLINE:
            ret="MONOLINE";
            break;
        case FUNCTION:
            ret="FUNCTION";
            break;
        case RETURNAL:
            ret="RETURN";
            break;
        case STRUCTURE:
            ret="STRUCTURE";
            break;
        case STRUCTDECLARE:
            ret="STRUCTUREDECLARATION";
            break;
        case MACRO:
            ret="MACRO";
            break;
        case ADD:
            ret="ADD";
            break;
        case SUBROUTINE:
            ret="SUBROUTINE";
            break;
        case LOOP:
            ret="LOOP";
            break;
        case IFC:
            ret="IF";
            break;
        case ELSEC:
            ret="ELSE";
            break;
        case FRAME:
            ret="FRAME";
            break;
        case MAIN:
            ret="MAIN";
            break;
        case DOT:
            ret="DOT";
            break;
        case COMMA:
            ret="COMMA";
            break;
        case OPENBRACKET:
            ret="OPENBRACKET";
            break;
        case CLOSEBRACKET:
            ret="CLOSEBRACKET";
            break;
        case OPENCURL:
            ret="OPENCURL";
            break;
        case CLOSECURL:
            ret="CLOSECURL";
            break;
        case OPENPARENTHESIS:
            ret="OPENPARENTHESIS";
            break;
        case CLOSEPARENTHESIS:
            ret="CLOSEPARENTHESIS";
            break;
        case COLON:
            ret="COLON";
            break;
        case LINEEND:
            ret="LINEEND";
            break;
        case EQUAL:
            ret="EQUAL";
            break;
        case SUM:
            ret="SUM";
            break;
        case SUBTRACT:
            ret="SUBTRACT";
            break;
        case MULTIPLY:
            ret="MULTIPLY";
            break;
        case DIVIDE:
            ret="DIVIDE";
            break;
        case MODULO:
            ret="MODULO";
            break;
        case AND:
            ret="AND";
            break;
        case OR:
            ret="OR";
            break;
        case XOR:
            ret="XOR";
            break;
        case LESS:
            ret="LESS";
            break;
        case MORE:
            ret="MORE";
            break;
        case SHIFTRIGHT:
            ret="SHIFTRIGHT";
            break;
        case SHIFTLEFT:
            ret="SHIFTLEFT";
            break;
        case INCREASE:
            ret="INCREASE";
            break;
        case DECREASE:
            ret="DECREASE";
            break;
        case NOT:
            ret="NOT";
            break;
        case DECIMALVALUE:
            ret="DECIMAL";
            break;
        case BINARY:
            ret="BINARY";
            break;
        case HEXADECIMAL:
            ret="HEXADECIMAL";
            break;
        case FIXEDPOINT:
            ret="FIXEDPOINT";
            break;
        case VOID:
            ret="VOID";
            break;
        case U8:
            ret="U8";
            break;
        case U16:
            ret="U16";
            break;
        case U32:
            ret="U32";
            break;
        case S8:
            ret="S8";
            break;
        case S16:
            ret="S16";
            break;
        case S32:
            ret="S32";
            break;
        case VOL:
            ret="Volatile";
            break;
        case ARR:
            ret="Array";
            break;
        case IDENT:
            ret="Identificador";
            break;
        case CHAIN:
            ret="String";
            break;
        case POINTER:
            ret="Pointer";
            break;
        case CHARACTER:
            ret="Char";
            break;
        case Nix:
            ret="Tipo Indeterminado";
        break;
    }
    return ret;

}

void Token::print(){
    std::cout
    <<"\nId:"<<id
    <<"\nLine:"<<line
    <<"\nString:"<<tokenString
    <<"\nToken:"<<stringer(type)
    <<"\nType:"<<string(subtype)
    <<std::endl;
}
std::string Token::str(){

    return "\n{Id:"+std::to_string(id)
    +" Line:"+std::to_string(line)
    +" String:"+tokenString
    +" Token:"+stringer(type)
    +" Type:"+string(subtype)+"}";
}
tokentype Token::checkMainType(){
    tokentype ret=NONE;
    for (const auto& kv : regexSuperType) {
        bool flag=std::regex_match(tokenString,regexSuperType[kv.first]);
        if(flag){
            ret=kv.first;
            break;
        }
    }

    return ret;
}

subtokentype Token::checkSubType(){
    subtokentype ret=Nix;
    for (const auto& kv : regexSubType) {
        bool flag=std::regex_match(tokenString,regexSubType[kv.first]);
        if(flag){
            ret=kv.first;
            break;
        }
    }

    return ret;
}
void Token::initializeTokenType(){
   try {
    type=checkMainType();
    subtype=checkSubType();
   } catch (std::regex_error& e) {
      std::cout << "Error!" << std::endl;
   }
}
