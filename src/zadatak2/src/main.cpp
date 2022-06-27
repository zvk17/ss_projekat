#include <iostream>
#include <CheckObjectFiles.hpp>
#include <MergeDataTables.hpp>
#include <Helper.hpp>
#include <MoveSections.hpp>
#include <PrintHex.hpp>
#include "ImportDataTable.hpp"
#include "OptReader.hpp"

int main(int argc, char *argv[]) {

    OptReader optReader(argc, argv);
    std::list<std::string> filePaths = optReader.getFilePaths();
    std::cout << "outfile: " << optReader.getOutputFile() << "\n";
    std::cout << "Linkable " << (int)optReader.isLinkable() << "; hex " << (int)optReader.isHex() << std::endl;

    if (optReader.isHex() && optReader.isLinkable()) {
        std::cout << "Bad args: required only one option -hex or -linkable";
        exit(__LINE__);
    }
    if (!optReader.isHex() && !optReader.isLinkable()) {
        std::cout << "Bad args: required only one option -hex or -linkable";
        exit(__LINE__);
    }

    std::list<DataTable*> dataTables;
    for(std::string filePath: filePaths) {
        DataTable* dt = ImportDataTable::import(filePath);
        dataTables.push_back(dt);
    }
    CheckObjectFiles cof(dataTables);
    if (optReader.isHex()) {
        cof.check();
    } else {
        cof.checkMultipleDefinitions();
    }
    MergeDataTables mdt(dataTables);

    DataTable* mergedTable = mdt.getMergedTable();

    //std::map<std::string, Section *> &sects = result->getSections();
    //std::map<std::string, Symbol *> &symbs = result->getSymbols();

    if (optReader.isLinkable()) {
        Helper::writeToFile(*mergedTable, optReader.getOutputFile());
        return 0;
    }

    MoveSections::checkValidity(*mergedTable, optReader.getSections());
    MoveSections::move(*mergedTable, optReader.getSections());

    PrintHex::printHex(*mergedTable, optReader.getOutputFile());
   // Helper::writeToFile(*mergedTable, optReader.getOutputFile());
    return 0;
}
