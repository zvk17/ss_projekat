//
// Created by user2 on 17.9.21..
//

#include <iostream>
#include "CheckObjectFiles.hpp"

CheckObjectFiles::CheckObjectFiles(const std::list<DataTable *> &tableList ) :
    tableList(tableList)
{}

void CheckObjectFiles::checkMultipleDefinitions() {
    for (DataTable* dt: tableList) {
        std::map<std::string, Symbol *> &symbols = dt->getSymbols();
        for (auto&& iter: symbols) {
            Symbol* symbol = iter.second;
            if (symbol->isGlobal() && !symbol->isSection()) {
                if (isInSet(symbol->getName())) {
                    std::cout << "Multiple definition error: symbol " << symbol->getName()  << std::endl;
                    exit(__LINE__);
                } else {
                    this->definedSymbolNames.insert(symbol->getName());
                }
            }
        }
    }
}

bool CheckObjectFiles::isInSet(std::string symbolName) {
    return definedSymbolNames.find(symbolName) != definedSymbolNames.end();;
}

void CheckObjectFiles::checkUndefinedSymbols() {
    for (DataTable* dt: tableList) {
        std::map<std::string, Symbol *> &symbols = dt->getSymbols();
        for (auto&& iter: symbols) {
            Symbol* symbol = iter.second;
            if (symbol->isExtern()) {
                if (!isInSet(symbol->getName())) {
                    std::cout << "Unresolved symbol error: symbol " << symbol->getName()  << " not defined" << std::endl;
                    exit(__LINE__);
                }
            }
        }
    }
}
const std::set<std::string>& CheckObjectFiles::getDefinedSymbolNames() const {
    return definedSymbolNames;
}

void CheckObjectFiles::check() {
    checkMultipleDefinitions();
    checkUndefinedSymbols();
}
