//
// Created by user2 on 15.9.21..
//

#ifndef ASSEMBLER_RELOCATION_HPP
#define ASSEMBLER_RELOCATION_HPP

#include "Symbol.hpp"
#include "Section.hpp"



class Relocation {

public:
    enum class RelocationType {
        PC_RELATIVE,
        ABSOLUTE_INSTRUCTION,
        LITTLE_ENDIAN_DATA
    };
private:
    Section* section;//pozicija
    int offset;//pozicija
    RelocationType relocationType;
    Symbol* symbol;//ako je lokalni simbol onda se kod ispisa ostavi ID sekcije
public:
    Relocation(Section *section, int offset, RelocationType relocationType, Symbol *symbol);

    int getOffset() const;
    void setOffset(int offset);
    RelocationType getRelocationType() const;
    void setRelocationType(RelocationType relocationType);
    Section *getSection() const;
    void setSection(Section *section);
    Symbol *getSymbol() const;
    void setSymbol(Symbol *symbol);


};


#endif //ASSEMBLER_RELOCATION_HPP
