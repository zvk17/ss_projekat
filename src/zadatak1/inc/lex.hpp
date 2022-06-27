//
// Created by user2 on 12.9.21..
//

#ifndef ASSEMBLER_LEX_HPP
#define ASSEMBLER_LEX_HPP


#include <cstdio>

extern int yylex();// extern "C"
extern int yylineno;
extern char* yytext;
extern FILE* yyin;
extern int yylex_destroy();// extern "C"

#endif //ASSEMBLER_LEX_HPP
