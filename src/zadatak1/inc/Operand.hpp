//
// Created by user2 on 12.9.21..
//

#ifndef ASSEMBLER_OPERAND_HPP
#define ASSEMBLER_OPERAND_HPP
#include "types.hpp"

class Operand {
public:
    enum class DataType {
        NONE,
        IMMEDIATE,
        MEMORY,
        PC_RELATIVE,
        REGISTER_DIRECT,
        REGISTER_INDIRECT,
        REGISTER_INDIRECT_DISPLACEMENT
    };
protected:
    DataType dataType;
    std::string symbolName;
    bool usingSymbol;
    RegisterType registerType;
    bool usingRegister;
    short literal;
    bool usingLiteral;

public:
    Operand();
    bool isUsingSymbol() const;
    bool isUsingRegister() const;
    bool isUsingLiteral() const;
    int getOperationSize() const;
    void setSymbolName(std::string name);
    void setRegisterType(RegisterType registerType);
    void setLiteral(short literal);
    void setDataType(DataType dataType);
    std::string toString() const;
    DataType getDataType() const;

    short getLiteral() const;

    std::string getSymbolName() const;

    RegisterType getRegisterType() const;
};


#endif //ASSEMBLER_OPERAND_HPP
