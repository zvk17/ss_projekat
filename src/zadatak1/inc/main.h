#ifndef MAIN_H
#define MAIN_H


#define GLOBAL_SYM 2
#define COMMA 3
#define ENDSYM 4
#define SECTION 5
#define SKIP 6
#define HALT 7
#define NEWLINE 8

#define RETSYM 10
#define IRETSYM 11
#define REG 12

#define PUSH_SYM 13
#define POP_SYM 14

#define COLON_SYM 15
#define TEXT_SYMB 16
#define EQUSYM 17
#define EXTERN_SYM 18
#define WORDSYM 19

#define CALL_SYM 209
#define JMP_SYM 220
#define JEQ_SYM 221
#define JNE_SYM 222
#define JGT_SYM 223

#define ADD_SYM 24
#define SUB_SYM 25
#define MUL_SYM 26
#define DIV_SYM 27
#define CMP_SYM 28

#define NOT_SYM 29
#define AND_SYM 30
#define OR_SYM 31
#define XOR_SYM 32
#define TEST_SYM 33

#define SHL_SYM 35
#define SHR_SYM 36

#define XCHG_SYM 37

#define LDR_SYM 38
#define STR_SYM 39

#define INTERRUPT_SYM 40

#define HEX_NUMBER 100
#define DOLLAR 101
#define PERCENT 102
#define STAR 103
#define NUMBER 104
#define PLUS 105
#define HASH 106
#define L_BRACKET 107
#define R_BRACKET 108


#define UNKNOWN -10000
#define IDENT   -30000



#endif
