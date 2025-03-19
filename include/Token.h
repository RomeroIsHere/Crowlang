#ifndef TOKEN_H
#define TOKEN_H
#include <string>
enum tokentype{
    COMMENT=0,
    RESERVED,
    PUNCTUATION,
    VALUE,
    TYPE,
    TYPEMOD,
    IDENTIFIER,
    NONE
};
enum subtokentype{
    MULTILINE=0,
    MONOLINE=1,//COMMENTS
    FUNCTION,
    RETURNAL,
    STRUCTURE,
    MACRO,
    ADD,
    SUBROUTINE,
    LOOP,
    FRAME,
    IFC,
    ELSEC,
    MAIN,//RESERVED KEYWORDS
    DOT,
    COMMA,
    OPENBRACKET,
    CLOSEBRACKET,
    OPENCURL,
    CLOSECURL,
    OPENPARENTHESIS,
    CLOSEPARENTHESIS,
    COLON,/*FUNC NAME(PARAM):TYPE:[MAIN,LOOP,FRAME]*/
    LINEEND,//pUNCTUATION
    EQUAL,
    SUM,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    MODULO,
    AND,
    OR,
    XOR,
    LESS,
    MORE,
    SHIFTRIGHT,
    SHIFTLEFT,//BINARY OPERATORS
    INCREASE,
    DECREASE,
    NOT,//UNARY
    CHAIN,
    CHARACTER,
    BINARY,
    HEXADECIMAL,
    FIXEDPOINT,
    DECIMALVALUE,//VALUES
    VOID,
    VOL,//for volatile
    ARR,
    POINTER,
    STRUCTDECLARE,
    U8,
    U16,
    U32,
    S8,
    S16,
    S32,//TYPE
    IDENT,
    Nix//none
};
struct Token
{
    public:
        Token();
        Token(std::string stringToken);
        Token(int id,int line,std::string stringToken);
        void print();
        void initializeTokenType();
        static void initialize();
        std::string str();
        static std::string string(subtokentype t);
        std::string tokenString;
        int id;
        int line;
        enum tokentype type;
        enum subtokentype subtype;
    private:
        tokentype checkMainType();
        subtokentype checkSubType();
};

#endif // TOKEN_H
