%{
#include "main.h"
%}
%option yylineno

digit         [0-9]
letter        [a-zA-Z_]
regDigit      [0-7]

%%
"#".*"\n"            { return HASH;       }

","                  { return COMMA;      }
":"                  { return COLON_SYM;  }
"$"                  { return DOLLAR;     }
"%"                  { return PERCENT;    }
"*"                  { return STAR;       }
"+"                  { return PLUS;       }
"["                  { return L_BRACKET;  }
"]"                  { return R_BRACKET;  }

"0x"[0-9a-fA-F]+     { return HEX_NUMBER; }
[+-]?[0-9]+               { return NUMBER;     }
".global"            { return GLOBAL_SYM;     }

".end"               { return ENDSYM;     }
".equ"               { return EQUSYM;     }
".extern"            { return EXTERN_SYM;  }
".word"              { return WORDSYM;    }
".section"           { return SECTION;    }
".skip"              { return SKIP;       }
"halt"               { return HALT;       }
"call"               { return CALL_SYM;       }
"ret"                { return RETSYM;     }
"iret"               { return IRETSYM;    }

"push"               { return PUSH_SYM;   }
"pop"                { return POP_SYM;    }


"jmp"                { return JMP_SYM;    }
"jeq"                { return JEQ_SYM;    }
"jne"                { return JNE_SYM;    }
"jgt"                { return JGT_SYM;    }

"add"                { return ADD_SYM;    }
"sub"                { return SUB_SYM;    }
"mul"                { return MUL_SYM;    }
"div"                { return DIV_SYM;    }
"cmp"                { return CMP_SYM;    }

"not"                { return NOT_SYM;    }
"and"                { return AND_SYM;    }
"or"                 { return OR_SYM;     }
"xor"                { return XOR_SYM;    }
"test"               { return TEST_SYM;   }

"shl"                { return SHL_SYM;    }
"shr"                { return SHR_SYM;    }

"xchg"               { return XCHG_SYM;   }

"ldr"                { return LDR_SYM;    }
"str"                { return STR_SYM;    }

r[0-7]               { return REG;        }
"psw"                { return REG;        }
"pc"                 { return REG;        }
"sp"                 { return REG;        }

"int"                { return INTERRUPT_SYM;}

"\n"                 { return NEWLINE;    }



[ \t\n\r]            /* skip whitespace */

{letter}({letter}|{digit})* {
                       //yylval.id = strdup(yytext);
                       return TEXT_SYMB;      }
.                    { /*printf("Unknown character [%c]\n",yytext[0]);*/
                  return UNKNOWN;    }
%%

int yywrap(void){return 1;}
