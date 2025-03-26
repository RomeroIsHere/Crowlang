#include <iostream>
#include <bits/stdc++.h>
#include "Translator.h"
#include <fstream>
#include <vector>
#include "HelperFunctions.h"
#include "Token.h"
Translator::Translator(std::string OutputName)
{
    Output=std::ofstream(OutputName,std::ofstream::out|std::ofstream::trunc);
}

Translator::~Translator()
{
    //dtor
}
std::string Translator::tillLine(std::vector<Token>* tokenIn, int* in){
    std::string line;
    std::vector<Token>& tokenList=*tokenIn;
    int ii=*in;
    while(tokenList[ii].subtype!=LINEEND){
        line+=tokenList[ii].tokenString+" ";
        ii++;
    }
        ii--;
    *in=ii;
    return line+"\n\t";
}

std::string Translator::ifcode(std::vector<Token>* tokenIn, int* in){

    std::vector<Token>& tokenList=*tokenIn;
    std::string condition;
    std::string codeblocked;
    std::string elsecodeblocked;
    bool haselse=false;

    int ii=*in;
    if(tokenList[ii++].subtype==IFC){
        if(tokenList[ii].subtype==OPENPARENTHESIS){
            do{
                condition+=tokenList[ii].tokenString;
                condition+=" ";
                ii++;
            }while(tokenList[ii].subtype!=CLOSEPARENTHESIS);
            condition+=tokenList[ii++].tokenString;
        }

        if(tokenList[ii].subtype==OPENCURL){

            codeblocked=codeblock(tokenIn,&ii);
        }else{
            tillLine(tokenIn,&ii);
        }
    }
    if(tokenList[++ii].subtype==ELSEC){
        haselse=true;
        do{
            ii++;
            if(tokenList[ii].subtype==OPENCURL){//inner codeBlock
                elsecodeblocked+=codeblock(tokenIn,&(ii));
                elsecodeblocked+=tokenList[++ii].tokenString;
            }else if (tokenList[ii].type==TYPEMOD||tokenList[ii].type==TYPE){//Type Declaration
                elsecodeblocked+=typedVariable(tokenIn,&ii);
                elsecodeblocked+=tokenList[ii].tokenString;
            }else if(tokenList[ii].subtype==IFC){//Control Statement
                elsecodeblocked+=ifcode(tokenIn,&ii);

            }else if(tokenList[ii].subtype==LOOP){//Loop Statement
                elsecodeblocked+=loopcode(tokenIn,&ii);
            }else{
                elsecodeblocked+=tokenList[ii].tokenString;
            }
            elsecodeblocked+=" ";
        }while(tokenList[ii].subtype!=LINEEND);
    }


    *in=ii;
    return "if "+condition+" "+codeblocked+";"+(haselse?+"else{"+elsecodeblocked+"}":";");
}

std::string Translator::loopcode(std::vector<Token>* tokenIn, int* in){
    std::string preloop;
    std::string postloop;
    std::string condition;
    std::vector<std::string> Cases;
    std::string codeblocked;
    std::vector<Token>& tokenList=*tokenIn;
    int ii=*in;
    int index=-1;
    if(tokenList[ii++].subtype==LOOP){
        if(tokenList[ii++].subtype==OPENPARENTHESIS){

            do{
                if(tokenList[ii].subtype==COLON){
                    switch(index){
                case -1:
                    preloop=condition+";";
                    break;
                default:
                    Cases.push_back(condition);
                    break;
                    }
                    index++;
                    condition.clear();
                }else{
                    condition+=tokenList[ii].tokenString;
                }
                ii++;
            }while(tokenList[ii].subtype!=CLOSEPARENTHESIS);
        }
        if(tokenList[++ii].subtype==OPENCURL){

            codeblocked=codeblock(tokenIn,&ii);
            codeblocked+=tokenList[++ii].tokenString;
        }else{
            codeblocked=tillLine(tokenIn,&ii);
        }
    }

    if(index>0){//if index is -1 or 0, it means there are no special instructions, so it won't run, otherwise it has special instrucitions
        preloop+=" u8 Index_Crow_Controlled=0;\n";
        postloop+="\nIndex_Crow_Controlled++;\nswitch(Index_Crow_Controlled){";
    }
    for(int ii=0; ii<index;ii++){
        //Add Instrucions to switch case
        //case 1: instruction1;break;
        //case 2:instruction2;break;
        postloop+="case "+std::to_string(ii+1)+":\n";
        postloop+=Cases[ii]+";\n";
        postloop+="break;\n";
    }
    if(index>0){//Finish the Processing of switch case
        preloop+=" u8 Index_Crow_Controlled=0;\n";
        postloop+="default:break;}";
    }


    *in=ii;

    return preloop+"while("+condition+"){{"+codeblocked+"}"+postloop+"}";
}


std::string Translator::typedVariable(std::vector<Token>* tokenIn, int* in){

    std::string name;
    std::string arrayer;
    std::string type;
    std::string referencer;
    std::vector<Token>& tokenList=*tokenIn;
    std::vector<Token> stacked;
    int ii=*in;
    if(tokenList[ii].type==TYPEMOD){
            do{
                switch(tokenList[ii].subtype){
                    case ARR:
                        referencer+="(";

                        do{
                            ii++;
                        if(tokenList[ii].subtype==OPENBRACKET)
                            stacked.push_back(tokenList[ii]);
                        if(tokenList[ii].subtype==CLOSEBRACKET){
                            stacked.pop_back();
                        }
                        arrayer+=tokenList[ii].tokenString;
                        }while(!stacked.empty());
                        arrayer+=")";
                        break;
                    case POINTER:
                        referencer+="*(";
                        arrayer+=")";
                        break;
                    case VOL:
                        type+="volatile ";
                        break;
                    case STRUCTDECLARE:
                        type+="struct ";
                        type+=tokenList[++ii].tokenString+" ";
                        break;
                    default:
                        std::cout<<"Something went Wrong"<<"\n";
                        tokenList[ii].print();

                }
                ii++;
            }while(tokenList[ii].type==TYPEMOD);
    std::reverse(referencer.begin(), referencer.end());
    }
    if(tokenList[ii].type==TYPE||tokenList[ii].type==IDENTIFIER){
        type+=tokenList[ii++].tokenString;
    }
    while(tokenList[ii].subtype==IDENT||tokenList[ii].subtype==DOT)
            name+=tokenList[ii++].tokenString;


    *in=ii;
    return type +" "+referencer+name+arrayer;
}

std::string Translator::codeblock(std::vector<Token>* tokenIn, int* in){
    std::string returnal;
    std::vector<Token>& tokenList=*tokenIn;
    int ii=*in;
    if(tokenList[ii].subtype==OPENCURL){
    returnal+=tokenList[ii].tokenString+"\n";
        do{
            ii++;
            if(tokenList[ii].subtype==OPENCURL){//inner codeBlock
                returnal+=codeblock(tokenIn,&(ii));
                returnal+=tokenList[++ii].tokenString;
            }else if (tokenList[ii].type==TYPEMOD||tokenList[ii].type==TYPE){//Type Declaration
                returnal+=typedVariable(tokenIn,&ii);
                returnal+=tokenList[ii].tokenString;
            }else if(tokenList[ii].subtype==IFC){//Control Statement
                returnal+=ifcode(tokenIn,&ii);

            }else if(tokenList[ii].subtype==LOOP){//Loop Statement
                returnal+=loopcode(tokenIn,&ii);
            }else{
                returnal+=tokenList[ii].tokenString;
            }
            returnal+=" ";
        }while(tokenList[ii].subtype!=CLOSECURL);

    //returnal+=tokenList[ii++].tokenString;
    }else{
        do{
            returnal+=tokenList[ii].tokenString;
            returnal+=" ";
        }while(tokenList[ii++].subtype!=LINEEND);

    }

    //returnal+="/*EndCodeblock*/"+tokenList[ii].tokenString+"\n";
    *in=ii;
    return returnal;
}
std::string Translator::function(std::vector<Token> *tokenIn, int *in){
    // function name([Optional Parameter Declaration])[:returnType[:routine]] statement[s or codeblock]
    std::vector<Token>& tokenList=*tokenIn;
    int ii=*in;
    std::string name;
    std::string returntype="void";
    std::string parameters;
    std::string referencer;
    std::string codeblocked;
    subtokentype routine=SUBROUTINE;

    name+=tokenList[ii+1].tokenString;
    int n=ii+2;
    do{
        if(tokenList[n].type==TYPEMOD||tokenList[n].type==TYPE){
            parameters+=typedVariable(&tokenList,&n);
            n--;
        }else{
            parameters+=tokenList[n].tokenString;
        }
        parameters+=" ";
    }while(tokenList[n++].subtype!=CLOSEPARENTHESIS);

    if(tokenList[n].subtype==COLON){

        returntype="";
        do{
                switch(tokenList[ii].subtype){
                    case ARR:
                    case POINTER:
                        referencer+="*";
                        break;
                    case VOL:
                        returntype+="volatile ";
                        break;
                    case STRUCTDECLARE:
                        returntype+="struct ";
                        returntype+=tokenList[++ii].tokenString+" ";
                        break;
                default:
                    returntype+=tokenList[++n].tokenString;
                    returntype+=" ";
                    break;
                }
        }while(tokenList[n+1].subtype!=COLON&&tokenList[n+1].subtype!=OPENCURL);
        if(tokenList[++n].subtype==COLON){

            routine=tokenList[++n].subtype;
            n++;
        }
    }
    codeblocked+="{";
    switch(routine){

    case MAIN:
        codeblocked+=replaceAll(replaceAll(replaceAll(parameters,",",";"),"(",""),")","");
        parameters="()";
        MainFunctions.push_back(name);
        codeblocked+=codeblock(&tokenList,&(n))+"}";

    break;

    case FRAME:
        codeblocked+=codeblock(&tokenList,&(n))+"vid_vsync();}";
    break;
    case SUBROUTINE:
    default:
    codeblocked+=codeblock(&tokenList,&(n))+"}";
    break;
    }
    *in=n;
    Definitions.push_back(returntype+" "+referencer+name+parameters+"/*"+Token::string(routine)+"*/"+codeblocked+"\n");
    return returntype+" "+referencer+name+parameters+";";;
}
std::string Translator::structConstruct(std::vector<Token> *tokenIn, int *in){
    // function name([Optional Parameter Declaration])[:returnType[:routine]] statement[s or codeblock]
    std::vector<Token>& tokenList=*tokenIn;
    int ii=*in;
    std::string name;
    std::string codeblocked;
    name+=tokenList[++ii].tokenString;
    codeblocked+=codeblock(&tokenList,&(++ii));
    *in=ii;
    return "struct "+name+" "+codeblocked+";\n";
}
void Translator::translate(std::vector<Token> tokenList){
    if(Output.bad()){
        return;
    }

    Output<<"#include <stdio.h>\n#include <tonc.h>\n";
    int n=tokenList.size();
    for(int ii=0;ii<n;ii++){
        if(tokenList[ii].subtype==FUNCTION){
            //Output<<"/*StartOfFunction*/\n";
            Output<<function(&tokenList,&ii);
            //Output<<"/*End of the Function*/\n";
        }else if(tokenList[ii].type==TYPEMOD||tokenList[ii].type==TYPE){
            //Output<<"/*Start of the TypedVariable till Line fromToken:"+tokenList[ii].str()+"*/\n";
            Output<<typedVariable(&tokenList,&ii);
            Output<<tillLine(&tokenList,&ii)<<";";
            //Output<<"/*End of the TypedVariable till Line*/\n";
        }else if(tokenList[ii].subtype==STRUCTURE){
            //Output<<"/*Start of Structure*/\n";
            Output<<structConstruct(&tokenList,&ii);
            //Output<<"/*End of the structure*/\n";
        }
        Output<<"\n";
    }
    Output<<"void main(){\n\t";
    for(std::string call:MainFunctions){
        Output<<call<<"();\n";
    }
    Output<<"while(1); return 0;}";
    for(std::string call:Definitions){
        Output<<call<<"\n";
    }

    Output.close();
}


