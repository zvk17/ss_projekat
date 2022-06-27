//
// Created by user2 on 16.9.21..
//

#ifndef ASSEMBLER_LINEEXCEPTION_HPP
#define ASSEMBLER_LINEEXCEPTION_HPP


#include "AssemblyException.hpp"

class LineException: public AssemblyException {
    static std::string buildMessage(int line, const std::string &message);

public:

    static const std::string SYMBOL_START_LINE;
    static const std::string SYMBOL_ALREADY_DEFINED;
    static const std::string DEFINING_EXTERN_SYMBOL;
    LineException(const std::string &message);


    static const std::string SYMBOL_GLOBAL_AND_EXTERN;
    static const std::string DEFINED_SYMBOL_EXTERN;
    static const std::string EXPECTED_COLON;
    static const std::string GLOBAL_UNDEFINED;
    static const std::string EXPECTED_IDENTIFIER;
    static const std::string EXPECTED_IDENTIFIER_OR_LITERAL;
    static const std::string OPERAND_WRONG_FORMAT;
    static const std::string EXPECTED_NEWLINE;
    static const std::string EXPECTED_R_BRACKET;
    static const std::string EXPECTED_REGISTER;
    static const std::string EXPECTED_COMMA;
    static const std::string EXPECTED_PLUS_OR_R_BRACKET;
    static const std::string EXPECTED_LITERAL;
    static const std::string EXPECTED_COMMA_OR_NEWLINE;
    static const std::string OUTSIDE_SECTION;
    static const std::string SYMBOL_NOT_DEFINED;
};


#endif //ASSEMBLER_LINEEXCEPTION_HPP
