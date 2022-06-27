//
// Created by user2 on 14.9.21..
//

#ifndef ASSEMBLER_FIRSTPASS_HPP
#define ASSEMBLER_FIRSTPASS_HPP

#include <string>
#include "DataTable.hpp"

class FirstPass {
private:

    void externSymbol();
    void textSymbol();
    Symbol * addSymbolWithValue(std::string symbolName);
    DataTable& dataTable;
    void globalSymbol(std::list<std::string>& symbolList);
public:
    FirstPass(DataTable& dataTable);
    void makePass(std::string filePath);


};


#endif //ASSEMBLER_FIRSTPASS_HPP
