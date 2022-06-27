//
// Created by user2 on 17.9.21..
//

#include "DataTable.hpp"
#include "MergeDataTables.hpp"

MergeDataTables::MergeDataTables(std::list<DataTable *> &tableList) :
    tableList(tableList)

{
     mergedTable = new DataTable();
     mergeSections();
     moveSymbols();
     moveUndefinedSymbols();
     moveRelocations();
}

void MergeDataTables::mergeSections() {
    for (DataTable* dt: tableList) {
        std::map<std::string, Section *> &sections = dt->getSections();
        for(auto&& iter: sections) {
            Section* section = iter.second;
            Section* mergedSection = mergedTable->getSection(section->getName());
            if (mergedSection == nullptr) {
                mergedTable->addSection(section->getName());
                mergedSection = mergedTable->getSection(section->getName());
                Symbol *symbol = new Symbol(section->getName());
                symbol->setGlobal(true);
                symbol->setIsSection(true);
                symbol->setSectionOwner(mergedSection);
                mergedTable->addSymbol(symbol);
            }
            section->setStartOffset(mergedSection->getSize());
            std::vector<byte>& sectionBytes = section->getByteCode();
            for(auto&&b: sectionBytes) {
                *mergedSection << b;
            }
            mergedSection->setSize(mergedSection->getByteCodeSize());
        }
    }
}

DataTable *MergeDataTables::getMergedTable()  {
    return mergedTable;
}

void MergeDataTables::moveSymbols() {
    for (DataTable* dt: tableList) {
        std::map<std::string, Symbol *> &symbols = dt->getSymbols();
        for (auto&& iter: symbols) {
            Symbol* oldSymbol = iter.second;
            if (!oldSymbol->isSection() && !oldSymbol->isExtern()) {
                Symbol* s = new Symbol(oldSymbol->getName());
                s->setGlobal(true);
                std::string sectionName = oldSymbol->getSectionOwner()->getName();
                s->setSectionOwner(mergedTable->getSection(sectionName));
                if (oldSymbol->isAbsolute()) {
                    s->setAbsolute();
                    s->setValue(oldSymbol->getValue());
                } else {
                    s->setValue(oldSymbol->getValue() + oldSymbol->getSectionOwner()->getStartOffset());
                }
                mergedTable->addSymbol(s);
            }
        }
    }
}
void MergeDataTables::moveUndefinedSymbols() {
    for (DataTable* dt: tableList) {
        std::map<std::string, Symbol *> &symbols = dt->getSymbols();
        for (auto&& iter: symbols) {
            Symbol* oldSymbol = iter.second;
            if (oldSymbol->isExtern()) {
                if (mergedTable->getSymbol(oldSymbol->getName()) == nullptr) {
                    Symbol* s = new Symbol(oldSymbol->getName());
                    s->setExtern(true);
                    mergedTable->addSymbol(s);
                }
            }
        }
    }
}

void MergeDataTables::moveRelocations() {
    for (DataTable* dt: tableList) {
        std::list<Relocation *> &relocations = dt->getRelocations();
        for(Relocation* r: relocations) {
            int newOffset = r->getSection()->getStartOffset() + r->getOffset();
            Section* newSection = mergedTable->getSection(r->getSection()->getName());
            Relocation::RelocationType rType = r->getRelocationType();
            Symbol* oldSymbol = r->getSymbol();
            Symbol* newSymbol = mergedTable->getSymbol(oldSymbol->getName());
            Relocation* rNew = new Relocation(
                    newSection,
                    newOffset,
                    rType,
                    newSymbol
            );
            mergedTable->addRelocation(rNew);
            std::vector<byte>& byteVector = newSection->getByteCode();
            if (oldSymbol->isSection()) {
                int lower = (int)(byteVector[newOffset] & 0xFF) & 0xFF;
                int upper = (int)(byteVector[newOffset + 1] & 0xFF) & 0xFF;
                short number = 0;
                if (Relocation::RelocationType::LITTLE_ENDIAN_DATA == rType) {
                    number = lower | (upper << 8);
                } else {
                    number = (lower << 8) | upper;
                }

                number += oldSymbol->getSectionOwner()->getStartOffset();


                if (Relocation::RelocationType::LITTLE_ENDIAN_DATA == rType) {
                    byteVector[newOffset] = (byte)(number & 0xFF);
                    byteVector[newOffset + 1] = (byte)( (number >> 8) & 0xFF);
                } else {
                    byteVector[newOffset + 1] = (byte)(number & 0xFF);
                    byteVector[newOffset] = (byte)( (number >> 8) & 0xFF);
                }
            }
        }
    }
}



