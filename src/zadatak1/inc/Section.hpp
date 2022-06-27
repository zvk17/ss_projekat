//
// Created by user2 on 12.9.21..
//

#ifndef ASSEMBLER_SECTION_HPP
#define ASSEMBLER_SECTION_HPP

#include <string>
#include <vector>
#include "types.hpp"

class Section {
private:
    std::string name;
    int startOffset;
    int size;

    std::vector<byte> byteCode;
public:
    Section(std::string name);

    int getStartOffset() const;

    void setStartOffset(int startOffset);
    int getEndOffset() const;
    std::string getName() const;

    void setSize(int size);
    int getSize() const;
    int getByteCodeSize() const;
    std::vector<byte>& getByteCode();

    friend Section& operator<<(Section& section, byte code);
    friend bool operator==(Section const&, Section const&);


};




#endif //ASSEMBLER_SECTION_HPP
