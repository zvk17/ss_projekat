//
// Created by user2 on 12.9.21..
//

#include <string>
#include "Operand.hpp"

int Operand::getOperationSize() const {
    switch (dataType) {
        case DataType::IMMEDIATE:
        case DataType::PC_RELATIVE:
        case DataType::REGISTER_INDIRECT_DISPLACEMENT:
        case DataType::MEMORY:
            return 5;
        case DataType::REGISTER_INDIRECT:
        case DataType::REGISTER_DIRECT:
            return 3;
        case DataType::NONE:
            throw "datatype not set";
    }
}

void Operand::setSymbolName(std::string name) {
    this->symbolName = name;
    this->usingSymbol = true;
}

void Operand::setRegisterType(RegisterType registerType) {
    this->registerType = registerType;
    this->usingRegister = true;
}
void Operand::setDataType(DataType dataType) {
    this->dataType = dataType;
}

void Operand::setLiteral(short literal) {
    this->literal = literal;
    this->usingLiteral = true;
}

Operand::Operand():
    dataType(DataType::NONE),
    symbolName(""),
    usingSymbol(false),
    registerType(RegisterType::R0),
    usingRegister(false),
    literal(0),
    usingLiteral(false)
{

}


bool Operand::isUsingRegister() const {
    return usingRegister;
}

bool Operand::isUsingLiteral() const {
    return usingLiteral;
}

bool Operand::isUsingSymbol() const {
    return usingSymbol;
}

std::string Operand::toString() const {
    char buffer [200];
    snprintf(buffer,
                    200,
                    "using symbol %d; using literal: %d, using register %d; size %d; literal: %d",
                    (int)usingSymbol,
                    (int)usingLiteral,
                    (int)usingRegister,
                    getOperationSize(),
                    literal
    );
    return std::string (buffer, 200);
}

Operand::DataType Operand::getDataType() const {
    return this->dataType;
}

RegisterType Operand::getRegisterType() const {
    if (!isUsingRegister()) {
        printf("not using register\n");
        throw "not using register";
    }
    return registerType;
}

short Operand::getLiteral() const {
    if (!isUsingLiteral()) {
        printf("not using literal\n");
        throw "not using literal";
    }
    return literal;
}

std::string Operand::getSymbolName() const {
    return symbolName;
}
