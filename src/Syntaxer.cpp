#include "Syntaxer.h"
#include <functional>
#include <iostream>

CrowError printer(){
    std::cout<<"------------I am Here!!"<<std::endl;
return CrowError();
}

CrowError Syntaxer::matchSubtype(Token t,subtokentype comp){
    if (t.subtype==comp){
        printerout<<t.tokenString<<" ";
        if(t.subtype==LINEEND)
        printerout<<std::endl;
        return CrowError();
    }else{
        //printer();
        if( maxDepth<=workingindex){
            maxDepth=workingindex;
            maxError=CrowError((int) comp,t);
            return maxError;
        }else{
            return CrowError((int) comp,t);
        }
    }
}

Syntaxer::Syntaxer(){
    workingindex=-1;
    max=0;
    maxError=CrowError();
    tokenlist=std::vector<Token>();
    maxDepth=0;
    printerout=std::ofstream("default.outstream");
}
Syntaxer::Syntaxer(std::vector<Token> inList){
    workingindex=0;
    stacker=0;
    max=inList.size();
    maxError=CrowError();
    maxDepth=0;
    tokenlist=inList;
    printerout=std::ofstream("default.outstream");
}
Syntaxer::Syntaxer(std::vector<Token> inList,std::string file){
    workingindex=0;
    stacker=0;
    max=inList.size();
    maxError=CrowError();
    tokenlist=inList;
    maxDepth=0;
    printerout=std::ofstream(file+".syntax");
}


CrowError Syntaxer::NoneOrOnce(std::function<CrowError()> functions[],int steps){
    stacker++;
    int getArrayLength =steps;
    CrowError nu;
    bool flag=true;
    int stack=workingindex;
    for(int i=0;i<getArrayLength&&flag;i++){
        nu=(functions[i]());
        if(nu.hasError()){
            flag=false;}
    }

    if(!flag){
            /*
            std::cout<<"NoneOrOnce:"<<std::endl;
            std::cout<<"    WorkIndex:"<<workingindex<<std::endl;
            std::cout<<"     Expected:"<<Token::string((subtokentype)nu.code)<<std::endl;
            std::cout<<"     id Error:"<<nu.errorId<<std::endl;
            std::cout<<"        stack:"<<stacker<<std::endl;
            tokenlist[nu.errorId].print();*/
        workingindex=stack;//REturn from Bad Parse into last Okay Position
    }

    stacker--;
    return CrowError();
}

CrowError Syntaxer::Once(std::function<CrowError()> functions[],int steps){
    stacker++;
    int getArrayLength =steps;
    CrowError nu;
    for(int i=0;i<getArrayLength;i++){
        nu=(functions[i]());

        if(nu.hasError()){/*
            std::cout<<"Once:"<<std::endl;
            std::cout<<"    WorkIndex:"<<workingindex<<std::endl;
            std::cout<<"    Expected:"<<Token::string((subtokentype)nu.code)<<std::endl;
            std::cout<<"    id Error:"<<nu.errorId<<std::endl;
            std::cout<<"        stack:"<<stacker<<std::endl;
            tokenlist[nu.errorId].print();*/
            break;
        }
    }
    stacker--;
    return nu;
}

CrowError Syntaxer::NoneOrMore(std::function<CrowError()> functions[],int steps){
    stacker++;
    int getArrayLength =steps;
    CrowError nu;
    bool flag=true;
    int stack;
    int i;
    while(flag&&workingindex<max){
        stack=workingindex;
        for(i=0;i<getArrayLength&&flag;i++){
            nu=((functions[i])());
            if(nu.hasError()){
                flag=false;
            }
        }
    }
    workingindex=stack;//REturn from Bad Parse into last Okay Position
    stacker--;
    if(i!=getArrayLength){
    /*std::cout<<"NoneOrMore:"<<std::endl;
            std::cout<<"    WorkIndex:"<<workingindex<<std::endl;
            std::cout<<"     Expected:"<<Token::string((subtokentype)nu.code)<<std::endl;
            std::cout<<"     id Error:"<<nu.errorId<<std::endl;
            std::cout<<"        stack:"<<stacker<<std::endl;
            tokenlist[nu.errorId].print();*/
    //return nu;
    }

    return CrowError();
}

CrowError Syntaxer::OneOf(std::function<CrowError()> functions[],int steps){
    stacker++;
    int getArrayLength =steps;
    CrowError nu;
    bool flag=true;
    int stack=workingindex;
        for(int i=0;i<getArrayLength&&flag;i++){
            nu=((functions[i])());

            if(nu.hasError()){
                workingindex=stack;//REturn from Bad Parse into last Okay Position
            }else{
                flag=false;
            }
        }
    /*if(flag){
    std::cout<<"OneOf:"<<std::endl;
            std::cout<<"    WorkIndex:"<<workingindex<<std::endl;
            std::cout<<"     Expected:"<<Token::string((subtokentype)nu.code)<<std::endl;
            std::cout<<"     id Error:"<<nu.errorId<<std::endl;
            std::cout<<"        stack:"<<stacker<<std::endl;
            tokenlist[nu.errorId].print();
    }*/
    stacker--;
    return nu;
}


CrowError Syntaxer::File(){
    std::function<CrowError()> functions[]={
    [this](){
        std::function<CrowError()> inner[]={
            std::bind(&Syntaxer::reserveddeclarations, this)
        };
        return NoneOrOnce(inner, 1);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],LINEEND);
    }
    };
    CrowError err;
    do{err=Once(functions,2);}while(!err.hasError()&&workingindex<max);
    return workingindex>=max?err:maxError;
    }

CrowError Syntaxer::IDENTIFIER(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],IDENT);
    },
    [this](){
        std::function<CrowError()> inner[]={
            [this](){
                return this->matchSubtype(this->tokenlist[this->workingindex++],DOT);
            },
            [this](){
                return this->matchSubtype(this->tokenlist[this->workingindex++],IDENT);
            }
        };
        return NoneOrMore(inner, 2);
    }
    };
    return Once(functions, 2);
    }
CrowError Syntaxer::reserveddeclarations(){
    std::function<CrowError()> functions[]={
    std::bind(&Syntaxer::inclusion, this),
    std::bind(&Syntaxer::macro, this),
    std::bind(&Syntaxer::structDeclaration, this),
    std::bind(&Syntaxer::funcDeclaration, this),
    std::bind(&Syntaxer::variabledeclaration, this)
    };
    return OneOf(functions,5);
    }
CrowError Syntaxer::inclusion(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],ADD);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],CHAIN);
    }
    };
    return Once(functions,2);
    }

CrowError Syntaxer::macro(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],MACRO);
    },
    std::bind(&Syntaxer::IDENTIFIER, this),
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],OPENPARENTHESIS);
    },
    [this](){
        std::function<CrowError()> inner[]={
        std::bind(&Syntaxer::IDENTIFIER, this),
        [this](){
            std::function<CrowError()> innerer[]={
                [this](){
                    return this->matchSubtype(this->tokenlist[this->workingindex++],COMMA);
                },
                std::bind(&Syntaxer::IDENTIFIER, this)
            };
            return NoneOrMore(innerer,2);
        }
        };
        return NoneOrOnce(inner,2);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],CLOSEPARENTHESIS);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],OPENPARENTHESIS);
    },
    [this](){
        std::function<CrowError()> inner[]={
            [this](){
                std::function<CrowError()> innerer[]={
                std::bind(&Syntaxer::statement, this),
                [this](){
                    return this->matchSubtype(this->tokenlist[this->workingindex++],CHAIN);
                }
                };
            return OneOf(innerer,2);
            }
        };
    return NoneOrMore(inner,1);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],CLOSEPARENTHESIS);
    }
    };
    return Once(functions,8);
    }

CrowError Syntaxer::structDeclaration(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],STRUCTURE);
    },
    std::bind(&Syntaxer::IDENTIFIER, this),
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],OPENCURL);
    },
    [this](){
        std::function<CrowError()> inner[]={
        std::bind(&Syntaxer::variabledeclaration, this),
        [this](){
        return this->matchSubtype(this->tokenlist[this->workingindex++],LINEEND);
        }
        };
    return NoneOrMore(inner,2);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],CLOSECURL);
    }
    };
    return Once(functions,5);
    }
CrowError Syntaxer::funcDeclaration(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],FUNCTION);
    },
    std::bind(&Syntaxer::IDENTIFIER, this),
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],OPENPARENTHESIS);
    },
    [this](){
        std::function<CrowError()> inner[]={
            std::bind(&Syntaxer::parameterdeclaration, this)
        };
        return NoneOrOnce(inner,1);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],CLOSEPARENTHESIS);
    },
    [this](){
        std::function<CrowError()> inner[]={
            [this](){
            return this->matchSubtype(this->tokenlist[this->workingindex++],COLON);
            },
            [this](){
                std::function<CrowError()> innerer[]={
                    std::bind(&Syntaxer::TYPEMOD, this)
                };
                return NoneOrOnce(innerer,1);
            },
            std::bind(&Syntaxer::TYPE, this),
            [this](){
                std::function<CrowError()> innerer[]={
                    [this](){
                    return this->matchSubtype(this->tokenlist[this->workingindex++],COLON);
                    },
                    std::bind(&Syntaxer::SCHEDULE, this)
                };
                return NoneOrOnce(innerer,2);;
            }
        };
    return NoneOrOnce(inner,4);
    },
    std::bind(&Syntaxer::statement, this)
    };

    return Once(functions,7);
    }


CrowError Syntaxer::parameterdeclaration(){
    std::function<CrowError()> functions[]={
        std::bind(&Syntaxer::variabledeclaration,this),
        [this](){
            std::function<CrowError()> inner[]={
                [this](){
                    return this->matchSubtype(this->tokenlist[this->workingindex++],COMMA);
                },
                std::bind(&Syntaxer::variabledeclaration,this)
            };
            return NoneOrMore(inner,2);
        }
    };
    return Once(functions,2);
    }

CrowError Syntaxer::SCHEDULE(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],SUBROUTINE);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],FRAME);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],MAIN);
    }
    };
    return OneOf(functions,3);
    }

CrowError Syntaxer::block(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],OPENCURL);
    },
    [this](){
        std::function<CrowError()> inner[]={
            std::bind(&Syntaxer::statement,this)
        };
        return NoneOrMore(inner,1);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],CLOSECURL);
    }
    };
    return Once(functions,3);
    }
CrowError Syntaxer::instruction(){
    std::function<CrowError()> innerer[]={
        std::bind(&Syntaxer::innerdeclarations,this),
        std::bind(&Syntaxer::assigner,this),
        std::bind(&Syntaxer::calculated,this),
        std::bind(&Syntaxer::returnal,this)
    };
    return OneOf(innerer,4);
}
CrowError Syntaxer::statement(){
    std::function<CrowError()> functions[]={
    [this](){
        std::function<CrowError()> inner[]={
        std::bind(&Syntaxer::instruction,this),
        [this](){
            return this->matchSubtype(this->tokenlist[this->workingindex++],LINEEND);
        }
        };
        return Once(inner,2);
    },
    std::bind(&Syntaxer::control,this),
    std::bind(&Syntaxer::block,this)
    };
    return OneOf(functions,3);
    }

CrowError Syntaxer::returnal(){
    std::function<CrowError()> functions[]={
        [this](){
            return this->matchSubtype(this->tokenlist[this->workingindex++],RETURNAL);
        },
        std::bind(&Syntaxer::calculated,this)
    };
    return Once(functions,2);
    }
CrowError Syntaxer::assigner(){
    std::function<CrowError()> functions[]={
        std::bind(&Syntaxer::IDENTIFIER,this),
        [this](){
            std::function<CrowError()> inner[]={
                [this](){
                    return this->matchSubtype(this->tokenlist[this->workingindex++],DOT);
                },
                std::bind(&Syntaxer::IDENTIFIER,this)
            };
            return NoneOrMore(inner,2);
        },
        [this](){
            std::function<CrowError()> inner[]={
                [this](){
                    return this->matchSubtype(this->tokenlist[this->workingindex++],OPENBRACKET);
                },
                std::bind(&Syntaxer::calculated,this),
                [this](){
                    return this->matchSubtype(this->tokenlist[this->workingindex++],CLOSEBRACKET);
                }
            };
            return NoneOrOnce(inner,3);
        },
        [this](){
            return this->matchSubtype(this->tokenlist[this->workingindex++],EQUAL);
        },
        [this](){
            std::function<CrowError()> inner[]={
                std::bind(&Syntaxer::calculated,this),
                std::bind(&Syntaxer::arrayc,this)
            };
            return OneOf(inner,2);
        }
    };
    return Once(functions,5);
    }
CrowError Syntaxer::control(){
    std::function<CrowError()> inner[]={
        std::bind(&Syntaxer::IFCODE,this),
        std::bind(&Syntaxer::LOOPCODE,this)
    };
    return OneOf(inner,2);
    }
CrowError Syntaxer::innerdeclarations(){
    std::function<CrowError()> inner[]={
        std::bind(&Syntaxer::initialization,this),
        std::bind(&Syntaxer::variabledeclaration,this)

    };
    return OneOf(inner,2);
    }
CrowError Syntaxer::variabledeclaration(){
    std::function<CrowError()> functions[]={
        [this](){
            std::function<CrowError()> inner[]={
                std::bind(&Syntaxer::TYPEMOD,this)
            };
            return NoneOrOnce(inner,1);
        },
        std::bind(&Syntaxer::TYPE,this),
        std::bind(&Syntaxer::IDENTIFIER,this)

    };
    return Once(functions,3);
    }
CrowError Syntaxer::initialization(){
    std::function<CrowError()> functions[]={
        [this](){
            std::function<CrowError()> inner[]={
                std::bind(&Syntaxer::TYPEMOD,this)
            };
            return NoneOrOnce(inner,1);
        },
        std::bind(&Syntaxer::TYPE,this),
        std::bind(&Syntaxer::assigner,this)
    };
    return Once(functions,3);
    }
CrowError Syntaxer::calculated(){
    std::function<CrowError()> functions[]={
        std::bind(&Syntaxer::term,this),
        [this](){
            std::function<CrowError()> inner[]={
                [this](){
                return this->matchSubtype(this->tokenlist[this->workingindex++],OPENPARENTHESIS);
                },
                std::bind(&Syntaxer::TYPE,this),
                [this](){
                return this->matchSubtype(this->tokenlist[this->workingindex++],CLOSEPARENTHESIS);
                },
                std::bind(&Syntaxer::calculated,this)
            };
            return Once(inner,4);
        }
    };
    return OneOf(functions,2);
    }
CrowError Syntaxer::term(){
    std::function<CrowError()> functions[]={
        std::bind(&Syntaxer::magnitude,this),
        [this](){
            std::function<CrowError()> inner[]={
                std::bind(&Syntaxer::comparative,this),
                std::bind(&Syntaxer::magnitude,this)
            };
            return NoneOrMore(inner,2);
        }
    };
    return Once(functions,2);
    }
CrowError Syntaxer::magnitude(){
    std::function<CrowError()> functions[]={
        std::bind(&Syntaxer::amount,this),
        [this](){
            std::function<CrowError()> inner[]={
                std::bind(&Syntaxer::bitshift,this),
                std::bind(&Syntaxer::amount,this)
            };
            return NoneOrMore(inner,2);
        }
    };
    return Once(functions,2);
    }
CrowError Syntaxer::amount(){
    std::function<CrowError()> functions[]={
        std::bind(&Syntaxer::factor,this),
        [this](){
            std::function<CrowError()> inner[]={
                std::bind(&Syntaxer::additive,this),
                std::bind(&Syntaxer::factor,this)
            };
            return NoneOrMore(inner,2);
        }
    };
    return Once(functions,2);
    }
CrowError Syntaxer::factor(){
    std::function<CrowError()> functions[]={
        std::bind(&Syntaxer::bits,this),
        [this](){
            std::function<CrowError()> inner[]={
                std::bind(&Syntaxer::multiplicative,this),
                std::bind(&Syntaxer::bits,this)
            };
            return NoneOrMore(inner,2);
        }
    };
    return Once(functions,2);
    }
CrowError Syntaxer::bits(){
    std::function<CrowError()> functions[]={
        std::bind(&Syntaxer::unarysigned,this),
        [this](){
            std::function<CrowError()> inner[]={
                std::bind(&Syntaxer::logical,this),
                std::bind(&Syntaxer::unarysigned,this)
            };
            return NoneOrMore(inner,2);
        }
    };
    return Once(functions,2);
    }
CrowError Syntaxer::unarysigned(){
    std::function<CrowError()> functions[]={
        [this](){
            std::function<CrowError()> inner[]={
                std::bind(&Syntaxer::UNARYOPERATOR,this),
                std::bind(&Syntaxer::OPERAND,this)
            };
            return Once(inner,2);
        },
        [this](){
            std::function<CrowError()> inner[]={
                std::bind(&Syntaxer::OPERAND,this),
                std::bind(&Syntaxer::UNARYOPERATOR,this)
            };
            return Once(inner,2);
        },
        std::bind(&Syntaxer::OPERAND,this)

    };
    return OneOf(functions,3);
    }
CrowError Syntaxer::OPERAND(){
    std::function<CrowError()> functions[]={
        std::bind(&Syntaxer::VALUE,this),
        [this](){
            return this->matchSubtype(this->tokenlist[this->workingindex++],CHAIN);
        },
        [this](){
            std::function<CrowError()> inner[]={
                std::bind(&Syntaxer::IDENTIFIER,this),
                [this](){
                return this->matchSubtype(this->tokenlist[this->workingindex++],OPENPARENTHESIS);
                },
                [this](){
                    std::function<CrowError()> innerer[]={
                        std::bind(&Syntaxer::calculated,this),
                        [this](){
                            std::function<CrowError()> innerest[]={
                                [this](){
                                return this->matchSubtype(this->tokenlist[this->workingindex++],COMMA);
                                },
                                std::bind(&Syntaxer::calculated,this)
                            };
                            return NoneOrMore(innerest,2);
                        }
                    };
                    return NoneOrOnce(innerer,2);
                },
                [this](){
                return this->matchSubtype(this->tokenlist[this->workingindex++],CLOSEPARENTHESIS);
                }
            };
            return Once(inner,4);
        },
        [this](){
            std::function<CrowError()> inner[]={
                std::bind(&Syntaxer::IDENTIFIER, this),
                [this](){
                return this->matchSubtype(this->tokenlist[this->workingindex++],OPENBRACKET);
                },
                std::bind(&Syntaxer::calculated,this),
                [this](){
                return this->matchSubtype(this->tokenlist[this->workingindex++],CLOSEBRACKET);
                }
            };
            return Once(inner,4);
        },
        std::bind(&Syntaxer::IDENTIFIER,this),
        [this](){
            std::function<CrowError()> inner[]={
                [this](){
                return this->matchSubtype(this->tokenlist[this->workingindex++],OPENPARENTHESIS);
                },
                std::bind(&Syntaxer::calculated,this),
                [this](){
                return this->matchSubtype(this->tokenlist[this->workingindex++],CLOSEPARENTHESIS);
                }
            };
            return Once(inner,3);
        }
    };
    return OneOf(functions,6);
    }

CrowError Syntaxer::comparative(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],LESS);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],MORE);
    }
    };
    return OneOf(functions,2);
    }

CrowError Syntaxer::bitshift(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],SHIFTRIGHT);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],SHIFTLEFT);
    }
    };
    return OneOf(functions,2);
    }
CrowError Syntaxer::additive(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],SUM);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],SUBTRACT);
    }
    };
    return OneOf(functions,2);
    }
CrowError Syntaxer::multiplicative(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],MULTIPLY);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],DIVIDE);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],MODULO);
    }
    };
    return OneOf(functions,3);
    }
CrowError Syntaxer::logical(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],AND);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],OR);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],XOR);
    }
    };
    return OneOf(functions,3);
    }

CrowError Syntaxer::IFCODE(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],IFC);
    },
    std::bind(&Syntaxer::calculated,this),
    std::bind(&Syntaxer::statement,this),
    [this](){
        std::function<CrowError()> inner[]={
            [this](){
                return this->matchSubtype(this->tokenlist[this->workingindex++],ELSEC);
            },
            std::bind(&Syntaxer::statement,this)
        };
    return NoneOrOnce(inner,2);
    }
    };
    return Once(functions,4);
    }
CrowError Syntaxer::LOOPCODE(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],LOOP);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],OPENPARENTHESIS);
    },
    [this](){
        std::function<CrowError()> inner[]={
            std::bind(&Syntaxer::instruction,this),
            [this](){
                return this->matchSubtype(this->tokenlist[this->workingindex++],COLON);
            }
        };
    return NoneOrMore(inner,2);
    },
    std::bind(&Syntaxer::calculated,this),
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],CLOSEPARENTHESIS);
    },
    std::bind(&Syntaxer::statement,this)
    };
    return Once(functions,6);
    }

CrowError Syntaxer::UNARYOPERATOR(){
    {
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],INCREASE);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],DECREASE);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],NOT);
    }
    };
    return OneOf(functions,3);
    }
    }
CrowError Syntaxer::VALUE(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],CHARACTER);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],DECIMALVALUE);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],BINARY);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],HEXADECIMAL);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],FIXEDPOINT);
    }
    };
    return OneOf(functions,5);
    }
CrowError Syntaxer::arrayc(){
    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],OPENBRACKET);
    },
    std::bind(&Syntaxer::calculated,this),
    [this](){
        std::function<CrowError()> inner[]={
            [this](){
                return this->matchSubtype(this->tokenlist[this->workingindex++],COMMA);
            },
            std::bind(&Syntaxer::calculated,this),
        };
        return NoneOrMore(inner, 2);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],CLOSEBRACKET);
    }
    };
    return Once(functions, 2);
    }
CrowError Syntaxer::TYPE(){

    std::function<CrowError()> functions[]={
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],VOID);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],U8);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],U16);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],U32);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],S8);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],S16);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],S32);
    },
    [this](){
    return this->matchSubtype(this->tokenlist[this->workingindex++],IDENT);
    }
    };
    return OneOf(functions,8);
    }
CrowError Syntaxer::TYPEMOD(){

    std::function<CrowError()> functions[]={
        [this](){
            std::function<CrowError()> inner[]={

                [this](){
                    return matchSubtype(this->tokenlist[this->workingindex++],VOL);
                }
            };
        return NoneOrOnce(inner,1);
        },
        [this](){
            std::function<CrowError()> inner[]={
                [this](){
                    return matchSubtype(this->tokenlist[this->workingindex++],STRUCTDECLARE);
                }
            };
        return NoneOrOnce(inner,1);
        },
        [this](){
            std::function<CrowError()> inner[]={
                [this](){
                    std::function<CrowError()> innerer[]={
                        [this](){
                            std::function<CrowError()> innerest[]={
                                [this](){
                                    return matchSubtype(this->tokenlist[this->workingindex++],POINTER);
                                }
                            };
                        return Once(innerest, 1);
                        },

                        [this](){
                            std::function<CrowError()> innerest[]={
                                [this](){
                                    return this->matchSubtype(this->tokenlist[this->workingindex++],ARR);
                                },
                                [this](){
                                    return this->matchSubtype(this->tokenlist[this->workingindex++],CLOSEBRACKET);
                                },
                                std::bind(&Syntaxer::calculated,this),
                                [this](){
                                    return this->matchSubtype(this->tokenlist[this->workingindex++],OPENBRACKET);
                                }
                            };
                            return Once(innerest, 4);
                        }
                    };
                    return OneOf(innerer,2);
                }
            };
        return NoneOrMore(inner,1);
        }
    };
    return Once(functions,3);
    }
CrowError Syntaxer::COMMENT(){
    return CrowError();
}

