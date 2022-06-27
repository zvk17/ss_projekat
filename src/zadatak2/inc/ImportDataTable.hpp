//
// Created by user2 on 17.9.21..
//

#ifndef LINKER_IMPORTDATATABLE_HPP
#define LINKER_IMPORTDATATABLE_HPP


#include <string>
#include "DataTable.hpp"

class ImportDataTable {
public:
    static DataTable* import(std::string file);
    static void readSection(DataTable& dt, std::ifstream& input);
    static void readSymbol(DataTable& table, std::ifstream& input);
    static void readRelocation(DataTable &table, std::ifstream &input);
};


#endif //LINKER_IMPORTDATATABLE_HPP
