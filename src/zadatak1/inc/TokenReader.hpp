//
// Created by user2 on 12.9.21..
//

#ifndef ASSEMBLER_TOKENREADER_HPP
#define ASSEMBLER_TOKENREADER_HPP

#include <string>
#include <list>
#include "types.hpp"
#include "Operand.hpp"


class TokenReader {
public:
    static std::list<std::string> readSymbolList();
    static RegisterOperation readTwoRegisters();
    static RegisterType readOneRegister();
    static RegisterType stringToRegisterType(std::string const& registerName);
    static std::pair<std::string, short> readEqu();
    static short readSkip();
    static short parseLiteralNumber(int token, std::string yytext);
    static int readNumberOfWords();
    static std::string readSectionName();
    static Operand readDataOperand();
    static Operand readAddressOperand();
    static void checkEndOfLine();
    static void checkClosingBracket();
    static void readBracketOperand(Operand &dataOperand);
    static std::pair<RegisterType, Operand> readDataInstruction();
};


#endif //ASSEMBLER_TOKENREADER_HPP
