//
// Created by user2 on 16.9.21..
//

#ifndef ASSEMBLER_HELPER_HPP
#define ASSEMBLER_HELPER_HPP


#include <string>
#include "DataTable.hpp"

class Helper {
public:
    static std::pair<std::string, std::string> getFilePathsFromArguments(int argc, char* argv[]);

    static void writeToFile(DataTable& dataTable, std::string outputFilePath);

    static void prettyPrintSymbol(Symbol *symbol, std::ostream& out);

    static void prettyPrintSectionBytes(Section *section, std::ostream &out);

    static void writeForLinker(DataTable& dataTable, std::string const& filePath);

    static std::string getSectionSymbolName(Symbol *symbol);

    static std::string getRelocationSymbolName(Symbol *symbol);

    static std::list<Symbol *> filterSectionSymbols(std::map<std::string, Symbol *> &symbols);

    static std::list<Symbol *> filterNonSectionSymbols(std::map<std::string, Symbol *> &symbols);

    static void printLinkerSymbol(Symbol *symbol, std::ostream& out);

    static void prettyPrintSection(Section *section, std::ostream &out);
};


#endif //ASSEMBLER_HELPER_HPP
