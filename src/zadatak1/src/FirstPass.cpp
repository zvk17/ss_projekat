//
// Created by user2 on 14.9.21..
//

#include "TokenReader.hpp"
#include "FirstPass.hpp"
#include "types.hpp"
#include "main.h"
#include "lex.hpp"
#include <cstdio>
#include <iostream>
#include "LineException.hpp"


void FirstPass::makePass(std::string filePath) {
    std::FILE *filePointer = nullptr;
    filePointer = std::fopen(filePath.c_str(),"r");
    if (filePointer == nullptr) {
        std::cout << "Could not open input file";
        exit(100);
    }
    yyin = filePointer;
    yylineno = 1;

    int token;
    short skip;
    int const WORD_SIZE = 2;
    int numberOfWords;
    std::string sectionName;
    std::pair<std::string, short> equ;
    std::pair<RegisterType, Operand> dataInstruction;
    Operand addressOperand;
    Symbol *symbol;
    token = yylex();
    std::list<std::string> symbolList, temp;

    while (token) {
        switch (token) {
            case GLOBAL_SYM:
                temp = TokenReader::readSymbolList();
                for (auto&&sName: temp) {
                    symbolList.push_back(sName);
                }
                break;
            case EXTERN_SYM:
                externSymbol();
                break;
            case TEXT_SYMB:
                textSymbol();
                token = yylex();
                if (token == TEXT_SYMB) {
                    throw LineException(LineException::SYMBOL_START_LINE);
                }
                continue;
                break;

            case ENDSYM:
                globalSymbol(symbolList);
                std::fclose(filePointer);
                return;
            case NEWLINE:
            case HASH:
                break;
            case SECTION:
                sectionName = TokenReader::readSectionName();
                dataTable.addSection(sectionName);
                symbol = addSymbolWithValue(sectionName);
                symbol->setIsSection(true);
                break;
            case SKIP:
                skip = TokenReader::readSkip();
                dataTable.incrementLocationCounterBy(skip);
                break;
            case EQUSYM: {
                equ = TokenReader::readEqu();
                Symbol* s = nullptr;
                Symbol* oldSymbol = dataTable.getSymbol(equ.first);
                if (oldSymbol == nullptr) {
                    s = new Symbol(equ.first);
                    s->setValue(equ.second);
                    s->setAbsolute();
                    dataTable.addSymbol(s);
                } else if (!oldSymbol->isDefined() && !oldSymbol->isExtern()) {
                    oldSymbol->setValue(equ.second);
                    oldSymbol->setAbsolute();
                } else if (oldSymbol->isDefined()){
                    throw LineException(LineException::SYMBOL_ALREADY_DEFINED);
                } else {
                    throw LineException(LineException::DEFINING_EXTERN_SYMBOL);
                }

                break;
            }
            case WORDSYM:
                numberOfWords = TokenReader::readNumberOfWords();
                dataTable.incrementLocationCounterBy(numberOfWords * WORD_SIZE);
                break;

            case ADD_SYM:
            case SUB_SYM:
            case MUL_SYM:
            case DIV_SYM:
            case CMP_SYM:

                TokenReader::readTwoRegisters();
                dataTable.incrementLocationCounterBy(2);
                break;
            case AND_SYM:
            case OR_SYM:
            case XOR_SYM:
            case TEST_SYM:
                TokenReader::readTwoRegisters();
                dataTable.incrementLocationCounterBy(2);
                break;
            case NOT_SYM:
                TokenReader::readOneRegister();
                dataTable.incrementLocationCounterBy(2);
                break;
            case XCHG_SYM:
            case SHL_SYM:
            case SHR_SYM:
                TokenReader::readTwoRegisters();
                dataTable.incrementLocationCounterBy(2);
                break;
            case JMP_SYM:
            case JEQ_SYM:
            case JNE_SYM:
            case JGT_SYM:
            case CALL_SYM:
                addressOperand = TokenReader::readAddressOperand();
                dataTable.incrementLocationCounterBy(addressOperand.getOperationSize());
                break;
            case HALT:
            case RETSYM:
            case IRETSYM:
                TokenReader::checkEndOfLine();
                dataTable.incrementLocationCounterBy(1);
                break;
            case PUSH_SYM:
            case POP_SYM:
                TokenReader::readOneRegister();
                dataTable.incrementLocationCounterBy(3);
                break;
            case INTERRUPT_SYM:
                TokenReader::readOneRegister();
                dataTable.incrementLocationCounterBy(2);
                break;
            case LDR_SYM:
            case STR_SYM:

                dataInstruction = TokenReader::readDataInstruction();
                dataTable.incrementLocationCounterBy(dataInstruction.second.getOperationSize());
                break;
        }

        token = yylex();
    }
    globalSymbol(symbolList);
    std::fclose(filePointer);
}

FirstPass::FirstPass(DataTable& dataTable):
    dataTable(dataTable)
{

}

void FirstPass::externSymbol() {

    std::list<std::string> symbolList = TokenReader::readSymbolList();
    for (std::string symbolName: symbolList) {
        Symbol* old = dataTable.getSymbol(symbolName);
        if (old == nullptr) {
            Symbol* s = new Symbol(symbolName);
            s->setExtern(true);
            dataTable.addSymbol(s);
        } else {
            if (old->isDefined()) {


                throw LineException(LineException::DEFINED_SYMBOL_EXTERN);
            }
            if (old->isGlobal()) {
                throw LineException(LineException::SYMBOL_GLOBAL_AND_EXTERN);
            }
            old->setExtern(true);
        }
    }
}

void FirstPass::textSymbol() {
    std::string symbolName(yytext);
    int token = yylex();
    if (token != COLON_SYM) {
        throw LineException(LineException::EXPECTED_COLON);
    }
    addSymbolWithValue(symbolName);

}
Symbol * FirstPass::addSymbolWithValue(std::string symbolName) {
    Symbol* oldSymbol = dataTable.getSymbol(symbolName);
    if (oldSymbol == nullptr) {
        Symbol* s = new Symbol(symbolName);
        s->setValue(dataTable.getLocationCounter());
        s->setSectionOwner(dataTable.getCurrentSection());
        dataTable.addSymbol(s);
        return s;
    } else {
        if (oldSymbol->isDefined()) {
            throw LineException(LineException::SYMBOL_ALREADY_DEFINED);

        }
        if (oldSymbol->isExtern()) {
            throw LineException(LineException::DEFINING_EXTERN_SYMBOL);
        }
        //nikad ne bi trebalo da se izvrsi
        throw "Something wrong addSymbolWithValue FirstPass.cpp";
        //oldSymbol->setValue(dataTable.getLocationCounter());
        //oldSymbol->setSectionIndex(dataTable.getCurrentSection()->getOrderNumber());
    }
}
void FirstPass::globalSymbol(std::list<std::string>& symbolList) {
    //printf("GLOBAL SYMBOL adding\n");
    for (std::string symbolName: symbolList) {
        Symbol* old = dataTable.getSymbol(symbolName);
        if (old == nullptr) {
            throw AssemblyException(LineException::GLOBAL_UNDEFINED + ": " + symbolName);
        } else {
            if (old->isExtern()) {
                throw AssemblyException(LineException::SYMBOL_GLOBAL_AND_EXTERN + ": " + symbolName);
            }
            old->setGlobal(true);
        }
    }
    symbolList.clear();
}