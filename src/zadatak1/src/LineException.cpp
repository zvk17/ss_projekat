//
// Created by user2 on 16.9.21..
//

#include "lex.hpp"
#include "LineException.hpp"

LineException::LineException(const std::string &message) :
    AssemblyException(buildMessage(yylineno, message))
{}

std::string LineException::buildMessage(int line, const std::string &message) {
    std::string msg = "Assembly exception on line ";
    msg += std::to_string(line);
    msg += ": ";
    msg += message;
    return msg;
}
const std::string LineException::SYMBOL_START_LINE = "Symbol must be at start of line";
const std::string LineException::SYMBOL_ALREADY_DEFINED = "Symbol already defined";
const std::string LineException::DEFINING_EXTERN_SYMBOL = "Defining extern symbol";
const std::string LineException::DEFINED_SYMBOL_EXTERN = "Defined symbol cant be extern";
const std::string LineException::SYMBOL_GLOBAL_AND_EXTERN = "Symbol cannot be global and extern";
const std::string LineException::EXPECTED_COLON = "Expected colon";
const std::string LineException::EXPECTED_NEWLINE = "Expected newline";
const std::string LineException::EXPECTED_REGISTER = "Expected register";
const std::string LineException::EXPECTED_COMMA = "Expected comma";
const std::string LineException::EXPECTED_COMMA_OR_NEWLINE = "Expected comma or newline";
const std::string LineException::EXPECTED_LITERAL = "Expected literal";
const std::string LineException::EXPECTED_R_BRACKET = "Expected right bracket";
const std::string LineException::EXPECTED_PLUS_OR_R_BRACKET = "Expected plus or right bracket";
const std::string LineException::GLOBAL_UNDEFINED = "Global symbol is undefined";
const std::string LineException::EXPECTED_IDENTIFIER = "Expected identifier";
const std::string LineException::EXPECTED_IDENTIFIER_OR_LITERAL = "Expected identifier or literal";
const std::string LineException::OPERAND_WRONG_FORMAT = "Wrong format of operand";
const std::string LineException::OUTSIDE_SECTION = "Outside section";
const std::string LineException::SYMBOL_NOT_DEFINED = "Symbol not defined or declared extern";