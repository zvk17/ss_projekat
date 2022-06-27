//
// Created by user2 on 12.9.21..
//

#ifndef ASSEMBLER_SYMBOL_HPP
#define ASSEMBLER_SYMBOL_HPP
#include <string>
#include "Section.hpp"

class Symbol {
private:
    std::string name;
    bool defined;
    bool global;
    bool externSymbol;
    short value;
    Section* sectionOwner;
    bool _isSection;
    bool _isAbsolute;
public:
    Symbol(std::string name);

    bool isDefined() const;
    bool isGlobal()  const;
    bool isExtern() const;
    bool isAbsolute() const;
    bool isSection() const;

    void setDefined(bool defined);
    void setGlobal(bool global);
    void setExtern(bool externSymbol);
    void setValue(short value);
    void setSectionOwner(Section *s);
    void setAbsolute();
    void setIsSection(bool isSection);

    std::string getName() const;
    short getValue() const;
    Section *getSectionOwner();
    std::string getFlagString() const;


};


#endif //ASSEMBLER_SYMBOL_HPP
