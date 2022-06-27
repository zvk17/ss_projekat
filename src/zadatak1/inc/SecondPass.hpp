//
// Created by user2 on 14.9.21..
//

#ifndef ASSEMBLER_SECONDPASS_HPP
#define ASSEMBLER_SECONDPASS_HPP


#include "DataTable.hpp"
#include "Relocation.hpp"

class SecondPass {
private:

    void aluInstruction(int operationToken, RegisterOperation registerOperation, Section *section);
    void jumpOperation(int operationToken, Operand addressOperand, Section *currentSection);
    void writeInstructionBytes(Section *pSection, short payloadBytes);
    DataTable& dataTable;
public:
    explicit SecondPass(DataTable& dataTable);
    void makePass(std::string filePath);
    void writeInstructionOperandBytes(Section *currentSection, Operand addressOperand);
    void dataInstruction(Section *currentSection, int operationToken, RegisterType registerType,
                         Operand dataOperand);

    void writeOperand(Section *currentSection, Operand dataOperand);
    void writeWords(Section* currentSection);
    void writeWordBytes(Section *currentSection, short number);
};


#endif //ASSEMBLER_SECONDPASS_HPP
