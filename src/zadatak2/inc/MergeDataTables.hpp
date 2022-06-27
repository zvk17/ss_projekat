//
// Created by user2 on 17.9.21..
//

#ifndef LINKER_MERGEDATATABLES_HPP
#define LINKER_MERGEDATATABLES_HPP

#include <string>
#include <list>

class MergeDataTables {
private:
    std::list<DataTable*> tableList;
    void mergeSections();
    DataTable *mergedTable;
public:
    MergeDataTables(std::list<DataTable *> &tableList);
    DataTable *getMergedTable();
    void moveSymbols();
    void moveRelocations();
    void moveUndefinedSymbols();
};


#endif //LINKER_MERGEDATATABLES_HPP
