//
// Created by user2 on 17.9.21..
//

#ifndef ASSEMBLER_PRETTYPRINTOUTPUT_HPP
#define ASSEMBLER_PRETTYPRINTOUTPUT_HPP


#include "Section.hpp"

class PrettyPrintOutput {

public:
    static void prettyPrintSection(Section *section, std::ostream &out);
    static void prettyPrintSymbol(Symbol *symbol, std::ostream &out);
    static void prettyPrintSectionBytes(Section *section, std::ostream &out);
};


#endif //ASSEMBLER_PRETTYPRINTOUTPUT_HPP
