//
// Created by user2 on 17.9.21..
//

#ifndef LINKER_CHECKOBJECTFILES_HPP
#define LINKER_CHECKOBJECTFILES_HPP


#include <set>
#include "DataTable.hpp"
/**
 * @note proverava da li su simboli visestruko definisani i da li su neki simboli ostali nedefinisani
 */
class CheckObjectFiles {
private:
    std::set<std::string> definedSymbolNames;
    std::list<DataTable*> tableList;
    bool isInSet(std::string symbolName);
    void checkUndefinedSymbols();
public:
    CheckObjectFiles(const std::list<DataTable *> &tableList);
    void checkMultipleDefinitions();

    const std::set<std::string>& getDefinedSymbolNames() const;
    void check();

};


#endif //LINKER_CHECKOBJECTFILES_HPP
