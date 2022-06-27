//
// Created by user2 on 14.9.21..
//
#include <cstdio>
#include <TokenReader.hpp>
#include <instruction_codes.hpp>
#include <iostream>
#include <LineException.hpp>
#include "lex.hpp"
#include "SecondPass.hpp"
#include "main.h"


SecondPass::SecondPass(DataTable& dataTable):
    dataTable(dataTable)
{}

void SecondPass::makePass(std::string filePath) {
    std::FILE *filePointer = nullptr;
    filePointer = std::fopen(filePath.c_str(),"r");
    if (filePointer == nullptr) {
        std::cout << "Could not open input file" << std::endl;
        exit(200);

    }
    yylex_destroy();
    yyin = filePointer;
    yylineno = 0;//hack

    int token;
    int operationToken;
    short skip;
    Operand addressOperand;
    RegisterOperation twoRegisters;
    std::string sectionName;
    Section* currentSection = nullptr;
    std::pair<RegisterType, Operand> dataInstructionPair;
    token = yylex();
    while (token) {
        switch (token) {
            case HASH:
            case NEWLINE:
                break;
            case ENDSYM:
                std::fclose(filePointer);
                return;
            case GLOBAL_SYM:
            case EXTERN_SYM:
                TokenReader::readSymbolList();
                break;
            case TEXT_SYMB:
                //todo textsymb
                break;
            case SECTION:
                sectionName = TokenReader::readSectionName();
                currentSection = dataTable.getSection(sectionName);
                break;
            case SKIP:
                skip = TokenReader::readSkip();
                while (skip--) {
                    *currentSection << (byte)0;
                }
                break;
            case EQUSYM:
                TokenReader::readEqu();
                break;
            case WORDSYM:
                writeWords(currentSection);
                break;
            case ADD_SYM:
            case SUB_SYM:
            case MUL_SYM:
            case DIV_SYM:
            case CMP_SYM:                
            case AND_SYM:
            case OR_SYM:
            case XOR_SYM:
            case TEST_SYM:
            case XCHG_SYM:
            case SHL_SYM:
            case SHR_SYM:
                operationToken = token;
                twoRegisters = TokenReader::readTwoRegisters();
                aluInstruction(operationToken, twoRegisters, currentSection);
                break;
            case NOT_SYM:
                if (currentSection == nullptr) {
                    throw LineException(LineException::OUTSIDE_SECTION);
                }
                twoRegisters.destination = TokenReader::readOneRegister();
                twoRegisters.source = RegisterType::R0;
                *currentSection << NOT_OPCODE << twoRegisters.toByte();
                break;
            case HALT:
                *currentSection << HALT_OPCODE;
                TokenReader::checkEndOfLine();
                break;
            case RETSYM:
                *currentSection << RET_OPCODE;
                TokenReader::checkEndOfLine();
                break;
            case IRETSYM:
                *currentSection << IRET_OPCODE;
                TokenReader::checkEndOfLine();
                break;
            case PUSH_SYM:
                twoRegisters.destination = TokenReader::readOneRegister();
                twoRegisters.source = RegisterType::SP;
                *currentSection
                    << STR_OPCODE
                    << twoRegisters.toByte()
                    << ((PRE_DECREMENT << 4) | REGISTER_INDIRECT);
                break;
            case POP_SYM:
                twoRegisters.destination = TokenReader::readOneRegister();
                twoRegisters.source = RegisterType::SP;
                *currentSection
                    << LDR_OPCODE
                    << twoRegisters.toByte()
                    << ((POST_INCREMENT << 4) | REGISTER_INDIRECT);
                break;
            case JMP_SYM:
            case JEQ_SYM:
            case JNE_SYM:
            case JGT_SYM:

                operationToken = token;
                addressOperand = TokenReader::readAddressOperand();
                jumpOperation(token, addressOperand, currentSection);
                break;
            case INTERRUPT_SYM:
                twoRegisters.destination = TokenReader::readOneRegister();
                twoRegisters.source = RegisterType::R0;
                *currentSection
                    << INTERRUPT_OPCODE
                    << (twoRegisters.toByte() | 0b1111);
                break;
            case LDR_SYM:
            case STR_SYM:
                operationToken = token;
                dataInstructionPair = TokenReader::readDataInstruction();
                dataInstruction(currentSection, operationToken, dataInstructionPair.first, dataInstructionPair.second);
                break;
        }

        token = yylex();
    }


    std::fclose(filePointer);
}

void SecondPass::aluInstruction(int operationToken, RegisterOperation registerOperation, Section *section) {
    if (section == nullptr) {
        throw LineException(LineException::OUTSIDE_SECTION);
    }
    switch (operationToken) {
        case ADD_SYM:
            *section << ADD_OPCODE;
            break;
        case SUB_SYM:
            *section << SUB_OPCODE;
            break;
        case MUL_SYM:
            *section << MUL_OPCODE;
            break;
        case DIV_SYM:
            *section << DIV_OPCODE;
            break;
        case CMP_SYM:
            *section << CMP_OPCODE;
            break;
        case AND_SYM:
            *section << AND_OPCODE;
            break;
        case OR_SYM:
            *section << OR_OPCODE;
            break;
        case XOR_SYM:
            *section << XOR_OPCODE;
            break;
        case TEST_SYM:
            *section << TEST_OPCODE;
            break;
        case XCHG_SYM:
            *section << XCHG_OPCODE;
            break;
        case SHL_SYM:
            *section << SHL_OPCODE;
            break;
        case SHR_SYM:
            *section << SHR_OPCODE;
            break;
        default:
            throw "Unexpected error aluInstr\n";
    }
    *section << registerOperation.toByte();
}

void SecondPass::jumpOperation(int operationToken, Operand addressOperand, Section* currentSection) {

    if (currentSection == nullptr) {
        throw LineException(LineException::OUTSIDE_SECTION);
    }
    short nextInstructionOffset = currentSection->getByteCodeSize() + 5;

    switch (operationToken) {
        case JMP_SYM:
            *currentSection << JMP_OPCODE;
            break;
        case JEQ_SYM:
            *currentSection << JEQ_OPCODE;
            break;
        case JGT_SYM:
            *currentSection << JGT_OPCODE;
            break;
        case JNE_SYM:
            *currentSection << JNE_OPCODE;
            break;
        case CALL_SYM:
            *currentSection << CALL_OPCODE;
            break;
        default:
            printf("Unespected operation token");
            throw "Unespected operation token";
    }
    byte regCode = 0b1111'0000;
    if (addressOperand.isUsingRegister()) {
        regCode |= (byte)addressOperand.getRegisterType();
    }
    *currentSection << regCode;
    writeOperand(currentSection, addressOperand);

}

void SecondPass::writeInstructionBytes(Section *pSection, short payloadBytes) {
    *pSection << (byte)((payloadBytes >> 8) & 0xFF) << (byte)(payloadBytes & 0xFF);
}
void SecondPass::writeWordBytes(Section *currentSection, short number) {
    *currentSection  << (byte)(number & 0xFF) << (byte)((number >> 8) & 0xFF);
}



void SecondPass::writeInstructionOperandBytes(
        Section *currentSection,
        Operand addressOperand
) {
    std::string symbolName = addressOperand.getSymbolName();
    Symbol* s = dataTable.getSymbol(symbolName);
    if (s == nullptr) {
        throw LineException(LineException::SYMBOL_NOT_DEFINED);
    }
    if (s->isAbsolute()) {
        //printf("writing absolute %s %d\n",s->getName().c_str(), s->getValue());
        writeInstructionBytes(currentSection, s->getValue());
        return;
    }
    dataTable.addRelocation(new Relocation(
            currentSection,
            currentSection->getByteCodeSize(),
            Relocation::RelocationType::ABSOLUTE_INSTRUCTION,
            s
    ));
    if (s->isExtern() || s->isGlobal()) {
        //printf("instruct 297 extern global\n");
        *currentSection << (byte)0 << (byte)0; //upisi prazno
    } else {
        writeInstructionBytes(currentSection, s->getValue());
    }
}

void SecondPass::dataInstruction(Section *currentSection, int operationToken, RegisterType registerType,
                                 Operand dataOperand) {
    if (currentSection == nullptr) {
        throw LineException(LineException::OUTSIDE_SECTION);
    }
    //printf("DATA INSTR\n");
    switch (operationToken) {
        case LDR_SYM:
            *currentSection << LDR_OPCODE;
            break;
        case STR_SYM:
            *currentSection << STR_OPCODE;
            break;
        default:
            throw "Should not be here";
    }
    byte regCode = 0;
    regCode |= ((byte)registerType << 4);
    if (dataOperand.isUsingRegister()) {
        regCode |= (byte)dataOperand.getRegisterType();
    } else if (dataOperand.getDataType() == Operand::DataType::PC_RELATIVE) {
        regCode |= (byte)RegisterType::PC;
    }
    *currentSection << regCode;
    writeOperand(currentSection, dataOperand);

}

void SecondPass::writeOperand(Section* currentSection, Operand dataOperand) {
    //if (dataOperand.isUsingSymbol()) {
    //    printf("write operand %s %d\n", dataOperand.getSymbolName().c_str(), dataOperand.getDataType());
    //}
    short literal;
    switch (dataOperand.getDataType()) {
        case Operand::DataType::IMMEDIATE:
        case Operand::DataType::MEMORY:
            if (Operand::DataType::IMMEDIATE == dataOperand.getDataType()) {
                *currentSection << (byte)((NO_INCREMENT << 4) | IMMEDIATE);
            } else {
                *currentSection << (byte)((NO_INCREMENT << 4) | MEMORY);
            }
            if (dataOperand.isUsingLiteral()) {
                writeInstructionBytes(currentSection, dataOperand.getLiteral());
            } else {
                writeInstructionOperandBytes(currentSection, dataOperand);
            }
            break;
        case Operand::DataType::REGISTER_DIRECT:
            *currentSection << (byte)((NO_INCREMENT << 4) | REGISTER_DIRECT);
            break;
        case Operand::DataType::REGISTER_INDIRECT:
            *currentSection << (byte)((NO_INCREMENT << 4) | REGISTER_INDIRECT);
            break;
        case Operand::DataType::REGISTER_INDIRECT_DISPLACEMENT:
            *currentSection << (byte)((NO_INCREMENT << 4) | REGISTER_INDIRECT_DISPLACEMENT);
            if (dataOperand.isUsingLiteral()) {
                writeInstructionBytes(currentSection, dataOperand.getLiteral());
            } else {
                writeInstructionOperandBytes(currentSection, dataOperand);
            }
            break;
        case Operand::DataType::PC_RELATIVE: {
            *currentSection << (byte)((NO_INCREMENT << 4) | REGISTER_INDIRECT_DISPLACEMENT);
            std::string symbolName = dataOperand.getSymbolName();
            Symbol *s = dataTable.getSymbol(symbolName);
            if (s == nullptr) {
                throw LineException(LineException::SYMBOL_NOT_DEFINED);
            }
            if (s->isAbsolute() || s->isExtern() || !(*s->getSectionOwner() == *currentSection)) {
                dataTable.addRelocation(new Relocation(
                        currentSection,
                        currentSection->getByteCodeSize(),
                        Relocation::RelocationType::PC_RELATIVE,
                        s
                ));
                if (s->isGlobal() || s->isExtern()) {
                    writeInstructionBytes(currentSection, -2);
                } else {
                    writeInstructionBytes(currentSection, s->getValue() - 2);
                }
            } else {
                writeInstructionBytes(currentSection, s->getValue() - currentSection->getByteCodeSize() - 2);
            }
            break;
        }
        break;

    }
}

void SecondPass::writeWords(Section *currentSection) {

    if (currentSection == nullptr) {
        throw LineException(LineException::OUTSIDE_SECTION);
    }
    int token;
    while (true) {
        token = yylex();
        if (NUMBER == token || HEX_NUMBER == token) {
            writeWordBytes(currentSection, TokenReader::parseLiteralNumber(token, yytext));
        }
        if (token == TEXT_SYMB) {
            std::string symbolName(yytext);
            Symbol *s = dataTable.getSymbol(symbolName);
            if (s == nullptr) {
                throw LineException(LineException::SYMBOL_NOT_DEFINED);
            }
            if (s->isAbsolute()) {
                writeWordBytes(currentSection, s->getValue());
                return;
            }
            dataTable.addRelocation(new Relocation(
              currentSection,
              currentSection->getByteCodeSize(),
                Relocation::RelocationType::LITTLE_ENDIAN_DATA,
                s
            ));

            if (s->isGlobal() || s->isExtern()) {
                *currentSection << (byte)0 << (byte)0;
            } else {
                writeWordBytes(currentSection, s->getValue());
            }
        }

        token = yylex();
        if (token == NEWLINE || token == HASH || token == 0) {
            break;
        } else if (token == COMMA) {
            continue;
        }
    }
}


