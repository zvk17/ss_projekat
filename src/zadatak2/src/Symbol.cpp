//
// Created by user2 on 12.9.21..
//

#include "Symbol.hpp"
Symbol::Symbol(std::string name):
        name(name),
        global(false),
        defined(false),
        externSymbol(false),
        sectionOwner(nullptr),
        _isAbsolute(false),
        _isSection(false),
        value(0)
{}

bool Symbol::isDefined() const {
    return defined;
}
bool Symbol::isGlobal() const {
    return global;
}
void Symbol::setDefined(bool defined) {
    this->defined = defined;
}
void Symbol::setGlobal(bool global) {
    this->global = global;
}

bool Symbol::isExtern() const {
    return this->externSymbol;
}

void Symbol::setExtern(bool externSymbol) {
    this->externSymbol = externSymbol;
}

void Symbol::setValue(short value) {
    this->value = value;
    setDefined(true);
}


std::string Symbol::getName() const {
    return name;
}

void Symbol::setAbsolute() {
    this->_isAbsolute = true;
    this->sectionOwner = nullptr;
}

short Symbol::getValue() const {
    return value;
}

Section* Symbol::getSectionOwner() {
    return sectionOwner;
}

void Symbol::setIsSection(bool isSection) {
    this->_isSection = isSection;
    if (isSection) {
        this->global = true;
    }
}

bool Symbol::isSection() const {
    return _isSection;
}

void Symbol::setSectionOwner(Section *s) {
    this->sectionOwner = s;
}

bool Symbol::isAbsolute() const {
    return _isAbsolute;
}

std::string Symbol::getFlagString() const {
    std::string flags;
    if (isSection()) {
        flags += "s";
    }
    if (isGlobal()) {
        flags += "g";
    } else if (isExtern()) {
        flags += "e";
    } else {
        flags += "l";
    }
    return flags;
}
