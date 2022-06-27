//
// Created by user2 on 12.9.21..
//

#include "Section.hpp"

std::string Section::getName() const {
    return this->name;
}
void Section::setSize(int size) {
    this->size = size;
}
int Section::getSize() const {
    return this->size;
}


Section::Section(std::string name) :
    name(name),
    size(0),
    startOffset(0)
{}




int Section::getByteCodeSize() const {
    return byteCode.size();
}

Section &operator<<(Section &section, byte code) {
    section.byteCode.push_back(code);
    return section;
}

std::vector<byte> &Section::getByteCode() {
    return byteCode;
}

bool operator==(Section const& s1, Section const& s2) {
    return s1.getName() == s2.getName();
}

int Section::getStartOffset() const {
    return startOffset;
}
int Section::getEndOffset() const {
    return startOffset + size;
}

void Section::setStartOffset(int startOffset) {
    Section::startOffset = startOffset;
}

