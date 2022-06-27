//
// Created by user2 on 17.9.21..
//

#include <iostream>
#include <fstream>
#include "ImportDataTable.hpp"
#include "types.hpp"

DataTable* ImportDataTable::import(std::string file) {
    DataTable* dataTable = new DataTable();
    DataTable& dt = *dataTable;
    std::ifstream input(file);
    if (!input.is_open()) {
        std::cout << "Could not open file: " << file << "\n";
        exit(__LINE__);
    }
    printf("Ucitavam %s\n", file.c_str());
    int sectionNumber;
    int symbolNumber;
    int relocationNumber;
    input >> sectionNumber;
    while (sectionNumber--) {
        readSection(dt, input);
    }
    input >> symbolNumber;
    while (symbolNumber--) {
        readSymbol(dt, input);
    }
    input >> relocationNumber;

    while (relocationNumber--) {
        readRelocation(dt, input);
    }

    input.close();
    return dataTable;
}

void ImportDataTable::readSection(DataTable& dt, std::ifstream& input) {
    short sectionSize;
    short sectionOffset;
    std::string sectionName;
    int data;
    byte dataByte;
    input >> sectionSize >> sectionOffset >> sectionName;
    //printf("ucitana sekcija: %s velicine: %d; offset: %d\n", sectionName.c_str(), sectionSize, sectionOffset);
    dt.addSection(sectionName);
    Section* s = dt.getSection(sectionName);
    s->setStartOffset(sectionOffset);
    s->setSize(sectionSize);
    for (int i = 0; i < sectionSize; i++) {
        input >> data;
        dataByte = (byte)(data & 0xFF);
        *s << dataByte;
    }
    //printf("last read %d\n", (int)dataByte);
}

void ImportDataTable::readSymbol(DataTable& table, std::ifstream &input) {
    std::string symbolName;
    std::string sectionName;
    bool isSection;
    bool isGlobal;
    bool isAbsolute;
    bool isExtern;
    int reader;
    int value;
    input >> symbolName >> sectionName >> value >> reader;
    isSection = (reader == 1);

    input >> reader;
    isGlobal = (reader == 1);

    input >> reader;
    isExtern = (reader == 1);

    input >> reader;
    isAbsolute = (reader == 1);


    Symbol* symbol = new Symbol(symbolName);
    if (isSection) {
        symbol->setIsSection(true);
    }
    if (isGlobal) {
        symbol->setGlobal(true);
    }
    if (isExtern) {
        symbol->setExtern(true);
    }
    if (isAbsolute) {
        symbol->setAbsolute();
    }
    if (!isAbsolute && !isExtern) {
        Section* section = table.getSection(sectionName);
        symbol->setSectionOwner(section);
    }
    if (!isExtern) {
        symbol->setValue((short)(value & 0xFFFF));
    }
    //printf ( "sSSymbol %s %d; G:%d;  S: %d\n", symbolName.c_str(), symbol->getValue(), symbol->isGlobal(),symbol->isSection());
    table.addSymbol(symbol);
}

void ImportDataTable::readRelocation(DataTable &table, std::ifstream &input) {

    int relocationType;
    int readOffset;
    std::string sectionName;
    std::string symbolName;
    input >> readOffset >> relocationType >> sectionName >> symbolName;
    Section* section = table.getSection(sectionName);
    Symbol* symbol = table.getSymbol(symbolName);
    //printf("relocation sec %s, sym %s:", sectionName.c_str(), symbolName.c_str());
    Relocation* relocation = new Relocation(
        section,
        readOffset,
        (Relocation::RelocationType)relocationType,
        symbol
    );
    table.addRelocation(relocation);
}
