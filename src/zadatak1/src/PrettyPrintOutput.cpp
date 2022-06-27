//
// Created by user2 on 17.9.21..
//

#include <Section.hpp>
#include <iomanip>
#include <Symbol.hpp>
#include "PrettyPrintOutput.hpp"
#include "Helper.hpp"
void PrettyPrintOutput::prettyPrintSection(Section* section, std::ostream& out) {
    out
            << std::left << std::setw(15) << std::setfill(' ') << section->getName() << std::right
            << std::string( 6, ' ' )
            << std::setw(4) << std::setfill('0') << std::hex << section->getSize()
            << std::string( 9, ' ' )
            << std::setw(4) << section->getStartOffset()
            << std::string( 12, ' ' )
            << std::setw(4) << section->getEndOffset()
            << std::setfill(' ')  << std::right
            << std::endl;
}
void PrettyPrintOutput::prettyPrintSymbol(Symbol *symbol, std::ostream& out) {
    std::string flags = symbol->getFlagString();
    std::string sectionName = Helper::getSectionSymbolName(symbol);
    out
            << std::setw(4) << std::setfill('0') << std::hex << ((int)symbol->getValue() & 0xFFFF)
            << std::string(3, ' ')
            << std::setw(7) << std::setfill(' ') << std::dec << flags
            << std::string(5, ' ')
            << std::setw(15) << std::left << symbol->getName()
            << std::string(3, ' ')
            << std::setw(15) << std::left << sectionName
            << std::right << "\n";
}
void PrettyPrintOutput::prettyPrintSectionBytes(Section * section, std::ostream& out) {
    out << "Section " << section->getName() << ":";
    int i = 0;
    std::vector<byte> &byteCode = section->getByteCode();

    for (auto&& byteIter: byteCode) {
        if (i % 8 == 0) {
            out << "\n" << std::setw(4) << std::setfill('0')  << i << ":  ";
        }
        out << std::setw(2) << std::setfill('0') << std::hex << ((int)byteIter &0xFF) << " ";
        i++;
    }
    out << "\n\n";
    out << std::setfill(' ');
}
