#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <regex>
#include "HelperFunctions.h"
#include "Token.h"
#include "Syntaxer.h"
#include "ErrorData.h"
#include "TypeChecker.h"
#include "Translator.h"
#include <windows.h>

std::vector<std::string> splitFull(std::string fullText){
    std::regex splitByGroup("(\".*\")|((0x|0h)[a-fA-F0-9_]+|0b[10_]+|0f[a-fA-F0-9]{2}\\.[a-fA-F0-9]{2})|[0-9]+|(/\\*(\n|\r|\n\r|.)*?\\*/|//.*)|[a-zA-Z][a-zA-Z0-9_]*|(<<?|>>?|\\+\\+?|\\{|\\}|\\[|\\]|\\(|\\)|=|\\-\\-?|\\*|/|%|&|\\||@|\\^|_|\\.|,|;|:|~)|\n|\r|\n\r");
    std::smatch res;
    std::vector<std::string> collection;
    std::string::const_iterator searchStart( fullText.cbegin() );
    while ( regex_search( searchStart, fullText.cend(), res, splitByGroup ) )//O(N regex N)
    {
        collection.push_back(res[0]);
        searchStart = res.suffix().first;//O(regex n)
    }
    return collection;
}

int main(int argc, char ** argv)
{
    Token::initialize();
    std::string mainFileName;
    std::string outputFileName;
    if(argc==2){
        mainFileName= argv[1];
        outputFileName= mainFileName+".c";
    }else if(argc>=3){
        mainFileName= argv[1];
        outputFileName= argv[2];
    }else{
        std::cout<<"Archivo a Compilar:"<<std::endl;
        std::cin>>mainFileName;
        std::cout<<"Archivo Destino:"<<std::endl;
        std::cin>>outputFileName;
    }
    std::cout<<"Se ha Empezado la Compilacion"<<std::endl;
    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    std::string commandline ="C:\\MinGW\\bin\\gcc.exe -xc -D add=#include -E -P "+ mainFileName +" -o "+ mainFileName+".start";
    CreateProcess( NULL,   // the path
    strdup(commandline.c_str()),        // Command line REM: This is Really ugly, why are Windows Calls so weird
    NULL,           // Process handle not inheritable
    NULL,           // Thread handle not inheritable
    FALSE,          // Set handle inheritance to FALSE
    0,              // No creation flags
    NULL,           // Use parent's environment block
    NULL,           // Use parent's starting directory
    &si,            // Pointer to STARTUPINFO structure
    &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );
    WaitForSingleObject(pi.hProcess,INFINITE);
    commandline ="C:\\MinGW\\bin\\gcc.exe -xc -D macro=#define -E -P "+ mainFileName +".start -o "+ mainFileName +".middle";
    CreateProcess( NULL,   // the path
    strdup(commandline.c_str()),        // Command line REM: This is Really ugly, why are Windows Calls so weird
    NULL,           // Process handle not inheritable
    NULL,           // Thread handle not inheritable
    FALSE,          // Set handle inheritance to FALSE
    0,              // No creation flags
    NULL,           // Use parent's environment block
    NULL,           // Use parent's starting directory
    &si,            // Pointer to STARTUPINFO structure
    &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );
    WaitForSingleObject(pi.hProcess,INFINITE);
    commandline ="C:\\MinGW\\bin\\gcc.exe -xc -E -P "+ mainFileName +".middle -o "+ mainFileName +".end";
    CreateProcess( NULL,   // the path
    strdup(commandline.c_str()),        // Command line REM: This is Really ugly, why are Windows Calls so weird
    NULL,           // Process handle not inheritable
    NULL,           // Thread handle not inheritable
    FALSE,          // Set handle inheritance to FALSE
    0,              // No creation flags
    NULL,           // Use parent's environment block
    NULL,           // Use parent's starting directory
    &si,            // Pointer to STARTUPINFO structure
    &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );
    WaitForSingleObject(pi.hProcess,INFINITE);
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
    mainFileName=mainFileName+".end";
    std::ifstream inputfile(mainFileName);
    try{
    if(!inputfile.good())
        throw std::runtime_error("No se encontro el archivo");
    }catch (int TALl){

    }

    std::vector<Token> tokenVect;
    int index=0;
    int line=1;
    std::stringstream buffer;

    buffer<<inputfile.rdbuf();

    for(std::string s:splitFull(buffer.str())){//o(2n)
        if(replaceAll(replaceAll(s,"\n",""),"\r","").length()!=0){//O(n)
            Token t=Token(index++,line,replaceAll(s,"\n"," "));//O(1)
            t.initializeTokenType();//O(Constant evaluation)
            if(t.subtype!=Nix&&t.subtype!=MONOLINE&&t.subtype!=MULTILINE)
                tokenVect.push_back(t);
            else{
                index--;
            }
        }
        line+=std::max((replaceAll(s,"\n","  ").length()-s.length()),(replaceAll(s,"\n\r","   ").length()-s.length()));
    }

    inputfile.close();
    bool tokenized=true;
    for(Token t:tokenVect){
        tokenized|=(t.type!=NONE)&(t.subtype!=Nix);
    }
    std::ofstream outfile(mainFileName+".crowTokens");
    if (!tokenized){
        outfile<<"TOKENIZER ERROR";
        return 1;
    }
    for(Token t:tokenVect){
        outfile<<t.str();
    }

    Syntaxer syni(tokenVect,mainFileName);
    CrowError stopped=syni.File();
    if(stopped.hasError()){
        std::cout<<"Codigo de Salida:"<<stopped.code<<std::endl;
        std::cout<<"Analisis Sintactico Fallido"<<std::endl;
        std::cout<<parseError(stopped)<<std::endl;
        return 2;
    }else{
        std::cout<<"Analisis Sintactico Exitoso"<<std::endl;
    }
    TypeChecker typo;
    stopped=typo.literalValueRanges(&tokenVect);
    if(stopped.hasError()){
        std::cout<<"Codigo de Salida:"<<stopped.code<<std::endl;
        std::cout<<"Analisis Semantico Fallido"<<std::endl;
        std::cout<<"Valor Fuera de Rango"<<std::endl;
        std::cout<<"Token:"<<stopped.tok.tokenString<<std::endl;
        std::cout<<"En Linea:"<<stopped.tok.line<<std::endl;
        return 3;
    }

    //Due to Semantic Analysis Being in Need Of Inclusion Statemens to work, And The Vastness of LibTonc, the Semantic Analysis Phase cannot be completed unless i translate the whole of LibTonc into crow Files
    //therefore, the Best Solution for Now is to delegate Semantic Analysis to the Gcc enabled compiler
    /*stopped=typo.findAllIdentifiers(tokenVect);
    if(stopped.hasError()){
            if(stopped.code==4){
        std::cout<<"Codigo de Salida:"<<stopped.code<<std::endl;
        std::cout<<"Analisis Semantico Fallido"<<std::endl;
        std::cout<<"Token No Definido"<<std::endl;
        std::cout<<"Token:"<<stopped.tok.tokenString<<std::endl;
        std::cout<<"En Linea:"<<stopped.tok.line<<std::endl;
        return 4;
        }else{
        std::cout<<"Codigo de Salida:"<<stopped.code<<std::endl;
        std::cout<<"Analisis Semantico Fallido"<<std::endl;
        std::cout<<"Accesso Incorrecto"<<std::endl;
        std::cout<<"Token:"<<stopped.tok.tokenString<<std::endl;
        std::cout<<"En Linea:"<<stopped.tok.line<<std::endl;
        return 5;
        }

    }*/
    std::cout<<"Analisis Semantico Exitoso"<<std::endl;
    Translator tora(outputFileName);
    std::cout<<"Escribiendo Archivo en:"<<outputFileName<<std::endl;
    tora.translate(tokenVect);
    std::cout<<"Se Ha Terminado de Escribir:"<<outputFileName<<std::endl;

    return 0;
}
