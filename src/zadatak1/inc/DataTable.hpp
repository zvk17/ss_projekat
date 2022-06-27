//
// Created by user2 on 12.9.21..
//

#ifndef ASSEMBLER_DATATABLE_HPP
#define ASSEMBLER_DATATABLE_HPP
#include <map>
#include <list>
#include "Section.hpp"
#include "Symbol.hpp"
#include "Relocation.hpp"

class DataTable {
    std::map<std::string, Section*> sections;
    std::list<Relocation*> relocations;
    std::map<std::string, Symbol*> symbols;
    Section* currentSection;
    short locationCounter;

public:
    DataTable();
    ~DataTable();
    void addRelocation(Relocation* relocation);
    int numberOfSections() const;
    void addSymbol(Symbol* symbol);
    Symbol* getSymbol(std::string symbolName);
    Section *getSection(std::string sectionName);
    void addSection(std::string sectionName);
    void incrementLocationCounterBy(short inc);
    short getLocationCounter() const;
    Section *getCurrentSection();
    std::map<std::string, Section *>& getSections() ;
    std::map<std::string, Symbol *>& getSymbols() ;
    std::list<Relocation *> &getRelocations();


};


#endif //ASSEMBLER_DATATABLE_HPP
