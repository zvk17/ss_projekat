//
// Created by user2 on 15.9.21..
//

#include "Relocation.hpp"
Symbol *Relocation::getSymbol() const {
    return symbol;
}

void Relocation::setSymbol(Symbol *symbol) {
    this->symbol = symbol;
}

Section *Relocation::getSection() const {
    return section;
}

void Relocation::setSection(Section *section) {
    this->section = section;
}

int Relocation::getOffset() const {
    return offset;
}

void Relocation::setOffset(int offset) {
    this->offset = offset;
}





Relocation::RelocationType Relocation::getRelocationType() const {
    return relocationType;
}

void Relocation::setRelocationType(Relocation::RelocationType relocationType) {
    this->relocationType = relocationType;
}

Relocation::Relocation(Section *section, int offset, Relocation::RelocationType relocationType,
                       Symbol *symbol) : section(section), offset(offset),
                                         relocationType(relocationType), symbol(symbol) {}


