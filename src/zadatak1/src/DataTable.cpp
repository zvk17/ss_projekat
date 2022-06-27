//
// Created by user2 on 12.9.21..
//

#include "DataTable.hpp"


#include <map>

int DataTable::numberOfSections() const {
    return this->sections.size();

}

void DataTable::addSection(std::string sectionName) {
    if (sections.find(sectionName) != sections.end()) {//sekcija vec postoji
        throw "Section with same name already exists";
    }
    locationCounter = 0;
    Section *section = new Section(sectionName);
    //section->setOrderNumber(numberOfSections());
    currentSection = section;
    sections.insert(std::pair<std::string, Section*>(sectionName, section));
    //printf("new section %s\n", sectionName.c_str());
}

void DataTable::incrementLocationCounterBy(short inc) {
    if (this->numberOfSections() == 0) {
        return;
    }
    locationCounter += inc;
    currentSection->setSize(locationCounter);
}

std::map<std::string, Section *>& DataTable::getSections() {
    return sections;
}

std::map<std::string, Symbol *>& DataTable::getSymbols() {
    return symbols;
}

DataTable::DataTable():
    locationCounter(0),
    sections(),
    currentSection(nullptr)
{

}

void DataTable::addSymbol(Symbol *symbol) {
    auto iterator = symbols.find(symbol->getName());
    if (iterator != symbols.end()) {
        throw "Error: already exist";
    }
    symbols.insert(std::pair<std::string, Symbol*>(symbol->getName(), symbol));
}

Symbol* DataTable::getSymbol(std::string symbolName) {
    auto iterator = symbols.find(symbolName);
    if (iterator == symbols.end()) {
        return nullptr;
    }
    return iterator->second;
}

Section *DataTable::getCurrentSection() {
    return currentSection;
}

short DataTable::getLocationCounter() const {
    return locationCounter;
}

DataTable::~DataTable() {
    for (auto iter: symbols) {
        delete iter.second;
    }
    for (auto iter: sections) {
        delete iter.second;
    }
    for (auto iter: relocations) {
        delete iter;
    }
    relocations.clear();
    symbols.clear();
    sections.clear();
    currentSection = nullptr;
    locationCounter = 0;
}

Section *DataTable::getSection(std::string sectionName) {
    auto iterator = sections.find(sectionName);
    if (iterator == sections.end()) {
        return nullptr;
    }
    return iterator->second;
}

void DataTable::addRelocation(Relocation *relocation) {
    relocations.push_back(relocation);
}

std::list<Relocation *> &DataTable::getRelocations() {
    return relocations;
}




