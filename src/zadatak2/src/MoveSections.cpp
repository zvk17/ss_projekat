//
// Created by user2 on 18.9.21..
//

#include <algorithm>
#include <iostream>
#include <set>
#include "MoveSections.hpp"
bool operator<(MoveSections::Interval const& a, MoveSections::Interval const&b) {
    if (a.start == b.start) {
        return a.end <= b.end;
    }
    return a.start <= b.start;
}

void MoveSections::checkValidity(DataTable &table, std::list<std::pair<std::string, short> > const& sections) {


    std::vector<Interval > intervalVector;
    for (auto&& iter: sections) {
        std::string sectionName = iter.first;
        int offset = iter.second;
        Section *s = table.getSection(sectionName);
        if (s == nullptr) {
            std::cout << "Section " << sectionName << ": from --place option does not exist.";
            exit(__LINE__);
        }
        Interval interval;
        interval.start = s->getStartOffset() + offset;
        interval.end = s->getEndOffset() + offset;
        interval.sectionName = sectionName;
        intervalVector.push_back(interval);
    }
    std::sort(intervalVector.begin(), intervalVector.end());
    for (int i = 0; i < intervalVector.size() - 1; i++) {
        Interval& i1 = intervalVector[i];
        Interval& i2 = intervalVector[i + 1];
        if (i1.end > i2.start) {
            std::cout << "Sections overlap: '" << i1.sectionName << "' and '" << i2.sectionName << "'";
            exit(__LINE__);
        }
        //printf("%s start %d end %d\n", interval.sectionName.c_str(), interval.start, interval.end);
    }

}

void MoveSections::moveSections(DataTable &table, std::list<std::pair<std::string, short>> const&sections) {
    int maxEndOffset = -1;
    std::set<std::string> sectionsDone;
    for (auto&&iter: sections) {
        std::string sectionName = iter.first;
        int offset = iter.second;
        Section* section = table.getSection(sectionName);
        int endOffset = section->getEndOffset() + offset;
        maxEndOffset = std::max(maxEndOffset, endOffset);
        section->setStartOffset(section->getStartOffset() + offset);

        sectionsDone.insert(sectionName);
    }

    maxEndOffset += 20;
    std::map<std::string, Section *> &allSections = table.getSections();
    for (auto&&iter: allSections) {
        std::string sectionName = iter.first;

        if (sectionsDone.find(sectionName) != sectionsDone.end()) {//ako nije nadjen
            continue;
        }

        sectionsDone.insert(sectionName);
        Section* section = iter.second;
        section->setStartOffset(maxEndOffset);
        maxEndOffset += section->getSize() + 100;
    }

}

void MoveSections::moveSymbols(DataTable &table) {
    std::map<std::string, Symbol *> &symbols = table.getSymbols();
    for (auto&&iter: symbols) {
        Symbol*s = iter.second;
        if (s->getSectionOwner() != nullptr) {
            int value = s->getValue();
            s->setValue(value + s->getSectionOwner()->getStartOffset());
        }
    }
}

void MoveSections::doRelocations(DataTable &table) {
    std::list<Relocation *> &relocations = table.getRelocations();
    for (auto&&r: relocations) {
        Section* section = r->getSection();
        int const newOffset = r->getOffset();
        std::vector<byte>& byteVector = section->getByteCode();
        int const value =  r->getSymbol()->getValue();
        int lower = (int)(byteVector[newOffset] & 0xFF) & 0xFF;
        int upper = (int)(byteVector[newOffset + 1] & 0xFF) & 0xFF;
        short number = 0;
        if (Relocation::RelocationType::LITTLE_ENDIAN_DATA == r->getRelocationType()) {
            number = lower | (upper << 8);
        } else {
            number = (lower << 8) | upper;
        }

        number += value;
        if (Relocation::RelocationType::PC_RELATIVE ==  r->getRelocationType()) {
            number -= section->getStartOffset() + newOffset;
        }

        if (Relocation::RelocationType::LITTLE_ENDIAN_DATA ==  r->getRelocationType()) {
            byteVector[newOffset] = (byte)(number & 0xFF);
            byteVector[newOffset + 1] = (byte)( (number >> 8) & 0xFF);
        } else {
            byteVector[newOffset + 1] = (byte)(number & 0xFF);
            byteVector[newOffset] = (byte)( (number >> 8) & 0xFF);
        }
    }

}

void MoveSections::move(DataTable& table, std::list<std::pair<std::string, short>> &sections) {
    MoveSections::moveSections(table, sections);
    MoveSections::moveSymbols(table);
    MoveSections::doRelocations(table);
}
