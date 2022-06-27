//
// Created by user2 on 16.9.21..
//

#include "Helper.hpp"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <PrettyPrintOutput.hpp>

std::pair<std::string, std::string> Helper::getFilePathsFromArguments(int argc, char **argv) {
    int opt;
    bool outputSet = false;
    std::string outputFile = "";
    std::string inputFile = "";

    while ( (opt = getopt(argc, argv, "o:")) != -1 ) {  // for each option...
        switch ( opt ) {
            case 'o':
                outputFile = "";
                outputFile += optarg;
                outputSet = true;
                break;
            default:
                std::cout << "Bad options" << std::endl;
                exit(-1);
        }
    }
    if (!outputSet) {
        std::cout << "Output file not set.\n";
        exit(100);
    }
    argc -= optind;
    argv += optind;
    if (argc > 0) {
        inputFile = argv[0];
    } else {
        std::cout << "All arguments required\n";
        std::exit(-1);
    }
    return std::pair<std::string, std::string>(inputFile, outputFile);
}
/**
 *
 * @param DataTable dt
 * @param outputFilePath
 * @note ispis pogodan za prikaz korisniku
 */
void Helper::writeToFile(DataTable& dt, std::string outputFilePath) {
    std::map<std::string, Section *> &sections = dt.getSections();
    std::map<std::string, Symbol *> &symbols = dt.getSymbols();
    std::list<Relocation*> relocations = dt.getRelocations();
    std::ofstream out(outputFilePath + ".txt");
    if (!out.is_open()) {
        std::cout << "Could not create output file\n";
        exit(200);
    }
    //ispis sekcija
    out << "Sections:\n" << "Name                 Size     Start offset     End offset" << std::endl;
    int id = 0;
    std::map<std::string, int> nameIndex;
    for (auto&& iter: sections) {
        Section* s = iter.second;
        PrettyPrintOutput::prettyPrintSection(s, out);
        nameIndex.insert({s->getName(), id++});
    }
    //ispis sekcija


    //<ispis simbola>
    out << "\nSymbols:\n" << "Value    Flags     Name              Section\n" ;
    //prvo sekcije
    for (Symbol* symbol: filterSectionSymbols(symbols)) {
        PrettyPrintOutput::prettyPrintSymbol(symbol, out);
    }
    //posle ostali simboli
    for (Symbol* symbol: filterNonSectionSymbols(symbols)) {
        PrettyPrintOutput::prettyPrintSymbol(symbol, out);
    }
    // </ispis sibola>

    //<ispis relokacija>
    //pretvaranje enuma Relocation::RelocationType u string za ispis
    std::map<Relocation::RelocationType, std::string> relocationName = {
            {Relocation::RelocationType::ABSOLUTE_INSTRUCTION,"ABSOLUTE_INSTRUCTION"},
            {Relocation::RelocationType::PC_RELATIVE,"PC_RELATIVE"},
            {Relocation::RelocationType::LITTLE_ENDIAN_DATA,"LITTLE_ENDIAN_DATA"}
    };

    out << "\n\nRelocations:\n";
    out << "Section            Offset     Type                       Symbol\n";
    for (auto&&iter: relocations) {
        Relocation* relocation = iter;
        Symbol* symbol = relocation->getSymbol();
        std::string symbolName = getRelocationSymbolName(symbol);
        std::string sectionName;

        out << std::setw(15)  << std::left << relocation->getSection()->getName()
            << std::string(5, ' ')
            << std::setw(4) << std::right << std::setfill('0') << std::hex << relocation->getOffset()
            << std::string(6, ' ')
            << std::setw(22)  << std::left << std::setfill(' ') << relocationName[relocation->getRelocationType()]
            << std::string(5, ' ')
            << std::setw(15) << std::left << std::setfill(' ') << symbolName
            << std::right << "\n"
        ;
    }
    //</ispis relokacija>


    //<ispis sadrzaja sekcija>
    out << "\n\nContents of sections:\n";
    for (auto&& iter: sections) {
        Section* section = iter.second;
        PrettyPrintOutput::prettyPrintSectionBytes(section, out);
    }
    //</ispis sadrzaja sekcija>

    out.close();
    //ispis fajla za ucitavanje u linker
    Helper::writeForLinker(dt, outputFilePath);
}


/**
 *
 * @param DataTable& dt
 * @param filePath
 * @note Ispis klase DataTable u formatu pogodnom za ucitavanje u linker
 */

void Helper::writeForLinker(DataTable& dt, std::string const& filePath) {
    std::map<std::string, Section *> &sections = dt.getSections();
    std::map<std::string, Symbol *> &symbols = dt.getSymbols();
    std::list<Relocation*> relocations = dt.getRelocations();

    std::ofstream out(filePath);
    if (!out.is_open()) {
        std::cout << "Could not create linker output file\n";
        exit(200);
    }


    //<ispis sekcija>
    out << sections.size() << std::endl;

    for (auto&&iter: sections) {
        Section* section = iter.second;
        // velicina_sekcije pocetak_sekcije ime_sekcije
        out << section->getSize() << " " << section->getStartOffset() << " " << section->getName() << std::endl;
        std::vector<byte> &bytes = section->getByteCode();
        for (auto&& byte: bytes) {
            out << ((int)byte & 0xFF) << " ";
        }
        out << std::endl;
    }
    out << std::endl;
    //<ispis sekcija>

    //<ispis simbola>
    int symbolNumber = 0;
    // prebroji koliko ima globalnih, eksternih, sekcija simbola
    for (auto&&iter: symbols) {
        Symbol* symbol = iter.second;
        if (symbol->isGlobal() || symbol->isExtern() || symbol->isSection()) {
            symbolNumber++;
        }
    }
    out << symbolNumber << std::endl;

    //ispisi sve sekcija simbole
    for (Symbol* symbol: filterSectionSymbols(symbols)) {
        Helper::printLinkerSymbol(symbol, out);
    }
    //ispisi sve globalne i externe simbole
    for (Symbol* symbol: filterNonSectionSymbols(symbols)) {
        if ( symbol->isGlobal() || symbol->isExtern() ) {
            Helper::printLinkerSymbol(symbol, out);
        }
    }
    //</ispis simbola>


    //<ispis relokacija>
    out << std::endl << relocations.size() << std::endl;//broj relokacija

    for (auto&&relocation: relocations) {
        //format: offset_relokacije vrsta_relokacije(int) ime_sekcije_za_patch  ime_simbola
        // za lokalne simbole na mestu prepravke fajla se nalazi vrednost simbola
        out << relocation->getOffset()
            << " " << (int)relocation->getRelocationType()
            << " " << relocation->getSection()->getName()
            << " " << getRelocationSymbolName(relocation->getSymbol())
            << "\n";
    }
    //</ispis relokacija>

    out.close();
}
std::string Helper::getSectionSymbolName(Symbol* symbol) {
    //ako simbol nije definisan ili je absolutan onda dobija hardkodovano ime sekcije
    std::string sectionName;
    if (symbol->isAbsolute()) {
        sectionName = "*ABS*";
    } else if (symbol->isExtern()) {
        sectionName = "*UND*";
    } else {
        sectionName = symbol->getSectionOwner()->getName();
    }
    return sectionName;
}

std::string Helper::getRelocationSymbolName(Symbol* symbol){
    std::string symbolName;
    if (!symbol->isGlobal() && !symbol->isExtern()) {
        if (symbol->isAbsolute()) {
            symbolName = "*ABS*";
        } else {
            symbolName = symbol->getSectionOwner()->getName();
        }

    } else {
            symbolName = symbol->getName();
    }

    return symbolName;
}
std::list<Symbol*> Helper::filterSectionSymbols(std::map<std::string, Symbol *> & symbols) {
    std::list<Symbol*> list;
    for (auto&&iter: symbols) {
        Symbol* symbol = iter.second;
        if (symbol->isSection()) {
            list.push_back(symbol);
        }
    }
    return list;
}

/**
 *
 * @param symbols
 * @return filtrirana lista simbola bez sekcija
 */
std::list<Symbol*> Helper::filterNonSectionSymbols(std::map<std::string, Symbol *> & symbols) {
    std::list<Symbol*> list;
    for (auto&&iter: symbols) {
        Symbol* symbol = iter.second;
        if (!symbol->isSection()) {
            list.push_back(symbol);
        }
    }
    return list;
}

void Helper::printLinkerSymbol(Symbol *symbol, std::ostream& out) {
    int val = symbol->getValue() & 0xFFFF;
    //printf("%s %d %d\n", symbol->getName().c_str(), symbol->getValue(),val);
    out << symbol->getName()
        << " " << getSectionSymbolName(symbol)
        << " " << val
        << " " << (int)symbol->isSection()
        << " " << (int)symbol->isGlobal()
        << " " << (int)symbol->isExtern()
        << " " << (int)symbol->isAbsolute()

        << std::endl;
}
