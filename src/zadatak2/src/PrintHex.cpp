//
// Created by user2 on 18.9.21..
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include "PrintHex.hpp"

void PrintHex::printHex(DataTable &table, std::string filePath) {
    std::ofstream out(filePath);
    if (!out.is_open()) {
        std::cout << "Could not open file for output\n";
        exit(__LINE__);
    }

    std::map<std::string, Section *> &sections = table.getSections();
    std::map<int,Section*> orderedSections;
    for (auto&&iter:sections) {
        Section* section = iter.second;
        orderedSections.insert({section->getStartOffset(), section});
    }
    for (auto&&iter:orderedSections) {
        Section* section = iter.second;
        std::vector<byte>& bytes = section->getByteCode();
        int const startOffset = section->getStartOffset();
        int val;
        int const size = bytes.size();
        int pos = - (startOffset % 8);
        //std::cout << "section " << section->getName() << " " << section->getStartOffset() << std::endl;

        while (pos < size) {
            out << std::hex << std::uppercase << std::setfill('0') << std::setw(4)
                << ((pos + startOffset) &0xFFFF) << ": ";

            for (int i = 0; i < 8; i++) {
                int j = i + pos;
                if (j < 0 || j > size) {
                    out << std::setw(2)  << 0 << " ";
                } else {
                    val = bytes[j];
                    out << std::setw(2) << val << " ";
                }
            }
            pos += 8;
            out << "\n";
        }
        out << "\n";

    }


    out.close();
}
