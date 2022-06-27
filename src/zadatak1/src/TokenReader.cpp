//
// Created by user2 on 12.9.21..
//

#include <LineException.hpp>
#include "TokenReader.hpp"
#include "types.hpp"
#include "lex.hpp"
#include "main.h"

std::list<std::string> TokenReader::readSymbolList() {
    std::list<std::string> symbolList;
    int token;
    while (true) {
        token = yylex();
        if (token != TEXT_SYMB) {
            throw LineException(LineException::EXPECTED_IDENTIFIER);
        }
        symbolList.push_back(std::string(yytext));
        token = yylex();
        if (token == NEWLINE || token == HASH || token == 0) {
            break;
        } else if (token == COMMA) {
            continue;
        } else {
            throw LineException(LineException::EXPECTED_COMMA_OR_NEWLINE);
        }
    }
    return symbolList;
}

RegisterOperation TokenReader::readTwoRegisters() {
    RegisterOperation ro;
    int token = yylex();
    if (token != REG) {
        throw LineException(LineException::EXPECTED_REGISTER);
    }
    ro.destination = TokenReader::stringToRegisterType(yytext);
    token = yylex();
    if (token != COMMA) {
        throw LineException(LineException::EXPECTED_COMMA);
    }
    token = yylex();
    if (token != REG) {
        throw LineException(LineException::EXPECTED_REGISTER);
    }
    ro.source = stringToRegisterType(yytext);
    checkEndOfLine();
    return ro;
}

RegisterType TokenReader::stringToRegisterType(std::string const& registerName) {
    if (registerName == "pc") {
        return RegisterType::PC;
    }
    if (registerName == "psw") {
        return RegisterType::PSW;
    }
    if (registerName == "sp") {
        return RegisterType::SP;
    }
    if (registerName.length() == 2 && registerName.at(0) == 'r') {
        char num = registerName.at(1) - '0';
        return static_cast<RegisterType>(num);
    }

}

std::pair<std::string, short> TokenReader::readEqu() {
    int token = yylex();
    if (token != TEXT_SYMB) {
        throw LineException(LineException::EXPECTED_IDENTIFIER);
    }
    std::string symbolName(yytext);
    short number = 0;
    token = yylex();
    if (token != COMMA) {
        throw LineException(LineException::EXPECTED_COMMA);
    }
    token = yylex();
    if (token != NUMBER && token != HEX_NUMBER) {
        throw LineException(LineException::EXPECTED_LITERAL);
    }
    number = parseLiteralNumber(token, yytext);
    checkEndOfLine();
    return std::pair<std::string, short>(symbolName, number);
}

short TokenReader::readSkip() {
    int token = yylex();
    if (token != NUMBER && token != HEX_NUMBER) {
        throw LineException(LineException::EXPECTED_LITERAL);
    }
    short literal = parseLiteralNumber(token, yytext);
    checkEndOfLine();
    return literal;
}
short TokenReader::parseLiteralNumber(int token, std::string yytext) {
    short number = 0;
    if (token == NUMBER) {
        number |= std::stoi(yytext) & 0xFFFF;
    } else {
        number = std::stoi(yytext, nullptr, 16);
    }
    return number;
}
int TokenReader::readNumberOfWords() {
    int count = 0;

    int token;
    while (true) {
        token = yylex();
        if (token != TEXT_SYMB && token != NUMBER && token != HEX_NUMBER) {
            throw LineException(LineException::EXPECTED_IDENTIFIER_OR_LITERAL);
        }
        count++;
        token = yylex();
        if (token == NEWLINE || token == HASH || token == 0) {
            break;
        } else if (token == COMMA) {
            continue;
        } else {
            throw LineException(LineException::EXPECTED_COMMA_OR_NEWLINE);
        }
    }
    return count;
}
std::string TokenReader::readSectionName() {
    int token = yylex();
    if (token != TEXT_SYMB) {
        throw LineException(LineException::EXPECTED_IDENTIFIER);
    }
    std::string sectionName(yytext);
    checkEndOfLine();
    return sectionName;
}

RegisterType TokenReader::readOneRegister() {
    int token = yylex();
    if (token != REG) {
        throw LineException(LineException::EXPECTED_REGISTER);

    }
    RegisterType registerType = TokenReader::stringToRegisterType(yytext);
    checkEndOfLine();
    return registerType;
}

Operand TokenReader::readDataOperand() {
    int token;
    Operand dataOperand;
    token = yylex();
    if (token == DOLLAR) {
        token = yylex();
        dataOperand.setDataType(Operand::DataType::IMMEDIATE);
        if (token == TEXT_SYMB) {
            dataOperand.setSymbolName(yytext);
        } else if (token == NUMBER || token == HEX_NUMBER) {
            short number = TokenReader::parseLiteralNumber(token, yytext);
            dataOperand.setLiteral(number);
        } else {
            throw LineException(LineException::EXPECTED_IDENTIFIER_OR_LITERAL);
        }
        checkEndOfLine();
        return dataOperand;
    }
    if (token == PERCENT) {
        token = yylex();
        if (token != TEXT_SYMB) {
            throw LineException(LineException::EXPECTED_IDENTIFIER);
        }
        dataOperand.setSymbolName(yytext);
        dataOperand.setDataType(Operand::DataType::PC_RELATIVE);
        checkEndOfLine();
        return dataOperand;
    }
    if (token == REG) {
        dataOperand.setDataType(Operand::DataType::REGISTER_DIRECT);
        dataOperand.setRegisterType(TokenReader::stringToRegisterType(yytext));
        checkEndOfLine();
        return dataOperand;
    }
    if (token == TEXT_SYMB) {
        dataOperand.setSymbolName(yytext);
        dataOperand.setDataType(Operand::DataType::MEMORY);
        checkEndOfLine();
        return dataOperand;
    }
    if (token == NUMBER || token == HEX_NUMBER) {
        dataOperand.setLiteral(TokenReader::parseLiteralNumber(token, yytext));
        dataOperand.setDataType(Operand::DataType::MEMORY);
        checkEndOfLine();
        return dataOperand;
    }
    if (token == L_BRACKET) {
        TokenReader::readBracketOperand(dataOperand);
        return dataOperand;
    }
    throw LineException(LineException::OPERAND_WRONG_FORMAT);
}

void TokenReader::checkEndOfLine() {
    int token = yylex();
    if (token != 0 && token != HASH && token != NEWLINE) {
        throw LineException(LineException::EXPECTED_NEWLINE);
    }
}

void TokenReader::checkClosingBracket() {
    int token = yylex();
    if (token != R_BRACKET) {
        throw LineException(LineException::EXPECTED_R_BRACKET);
    }
    checkEndOfLine();
}


Operand TokenReader::readAddressOperand() {
    int token;
    token = yylex();
    Operand addressOperand;
    if (token == PERCENT) {
        addressOperand.setDataType(Operand::DataType::PC_RELATIVE);
        token = yylex();
        if (token != TEXT_SYMB) {
            throw LineException(LineException::EXPECTED_IDENTIFIER);
        }
        addressOperand.setSymbolName(yytext);
        checkEndOfLine();
        return addressOperand;
    }

    if (token == NUMBER || token == HEX_NUMBER) {
        addressOperand.setLiteral(TokenReader::parseLiteralNumber(token, yytext));
        addressOperand.setDataType(Operand::DataType::IMMEDIATE);
        checkEndOfLine();
        return addressOperand;
    }
    if (token == TEXT_SYMB) {
        addressOperand.setDataType(Operand::DataType::IMMEDIATE);
        addressOperand.setSymbolName(yytext);
        checkEndOfLine();
        return addressOperand;
    }
    if (token == STAR) {
        token = yylex();
        if (token == REG) {
            addressOperand.setDataType(Operand::DataType::REGISTER_DIRECT);
            addressOperand.setRegisterType(TokenReader::stringToRegisterType(yytext));
            checkEndOfLine();
            return addressOperand;
        }
        if (token == TEXT_SYMB) {
            addressOperand.setDataType(Operand::DataType::MEMORY);
            addressOperand.setSymbolName(yytext);
            checkEndOfLine();
            return addressOperand;
        }
        if (token == NUMBER || token == HEX_NUMBER) {
            addressOperand.setLiteral(TokenReader::parseLiteralNumber(token, yytext));
            addressOperand.setDataType(Operand::DataType::MEMORY);
            checkEndOfLine();
            return addressOperand;
        }
        if (token == L_BRACKET) {
            readBracketOperand(addressOperand);
            return addressOperand;
        }
    }
    throw LineException(LineException::OPERAND_WRONG_FORMAT);
}
void TokenReader::readBracketOperand(Operand & dataOperand) {
    int token;
    token = yylex();
    if (token != REG) {
        throw LineException(LineException::EXPECTED_REGISTER);
    }
    dataOperand.setRegisterType(TokenReader::stringToRegisterType(yytext));
    token = yylex();
    if (token == R_BRACKET) {
        checkEndOfLine();
        dataOperand.setDataType(Operand::DataType::REGISTER_INDIRECT);
        return;// dataOperand;
    } else if (token == PLUS) {
        token = yylex();
        dataOperand.setDataType(Operand::DataType::REGISTER_INDIRECT_DISPLACEMENT);
        if (token == TEXT_SYMB) {
            dataOperand.setSymbolName(yytext);
        } else if (token == NUMBER || token == HEX_NUMBER) {
            dataOperand.setLiteral(TokenReader::parseLiteralNumber(token,yytext));

        } else {
            throw LineException(LineException::EXPECTED_IDENTIFIER_OR_LITERAL);
        }
        checkClosingBracket();
        return;
    } else {
        throw LineException(LineException::EXPECTED_PLUS_OR_R_BRACKET);
    }
}

std::pair<RegisterType, Operand> TokenReader::readDataInstruction() {
    int token = yylex();
    if (token != REG) {
        throw LineException(LineException::EXPECTED_REGISTER);
    }
    RegisterType registerType = stringToRegisterType(yytext);
    token = yylex();
    if (token != COMMA) {
        throw LineException(LineException::EXPECTED_COMMA);
    }
    Operand dataOperand = readDataOperand();
    return std::pair<RegisterType, Operand>(registerType, dataOperand);
}
