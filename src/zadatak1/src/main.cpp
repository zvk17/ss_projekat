#include <cstdio>
#include "main.h"
#include <list>
#include <string>
#include <cstring>
#include "DataTable.hpp"
#include "FirstPass.hpp"
#include "main.h"
#include "types.hpp"
#include "lex.hpp"
#include "TokenReader.hpp"
#include <iostream>
#include <Helper.hpp>
#include <AssemblyException.hpp>
#include "SecondPass.hpp"


int main(int argc, char *argv[]) {
    std::pair<std::string, std::string> filePaths = Helper::getFilePathsFromArguments(argc, argv);
    std::string inputFile = filePaths.first;
    std::string outputFile = filePaths.second;
    //printf("|%s| |%s|\n", inputFile.c_str(), outputFile.c_str());

    DataTable dt;
    FirstPass fp(dt);
    SecondPass sp(dt);
    try {
        fp.makePass(inputFile);
        sp.makePass(inputFile);
    } catch (AssemblyException const& assemblyException) {
        std::cout << assemblyException.getMessage() << std::endl;
        return 10;
    }

    Helper::writeToFile(dt, outputFile);

    return 0;
}
